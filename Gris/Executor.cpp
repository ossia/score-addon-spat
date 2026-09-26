#include <Process/Dataflow/Cable.hpp>
#include <Process/ExecutionContext.hpp>
#include <Process/ExecutionSetup.hpp>
#include <Process/ExecutionTransaction.hpp>

#include <score/document/DocumentContext.hpp>
#include <score/tools/Bind.hpp>

#include <ossia/dataflow/execution_state.hpp>
#include <ossia/dataflow/graph/graph_interface.hpp>
#include <ossia/dataflow/graph_node.hpp>
#include <ossia/dataflow/port.hpp>
#include <ossia/detail/algorithms.hpp>
#include <ossia/detail/math.hpp>
#include <ossia/network/value/value_conversion.hpp>

#include <QByteArray>
#include <QTimer>

#include <Gris/Algo/Spatializer.hpp>
#include <Gris/Algo/SpeakerSetupIO.hpp>
#include <Gris/Executor.hpp>
#include <Gris/LiveSources.hpp>
#include <Gris/SpeakerList.hpp>

#include <cmath>

#include <atomic>
#include <deque>
#include <memory>
#include <mutex>
#include <vector>

namespace Gris
{
namespace
{
[[nodiscard]] ossia::value const* lastValue(ossia::value_inlet const& inlet) noexcept
{
  auto const& data = inlet.data.get_data();
  if(data.empty())
    return nullptr;
  return &data.back().value;
}
}

class SpatNode final : public ossia::nonowning_graph_node
{
public:
  struct Ports
  {
    std::deque<ossia::audio_inlet> audio;
    std::deque<ossia::value_inlet> values;
  };

  [[nodiscard]] static std::shared_ptr<Ports> makePorts(int sourceCount)
  {
    auto p = std::make_shared<Ports>();
    for(int s = 0; s < sourceCount; ++s)
    {
      p->audio.emplace_back();
      for(int k = 1; k < SpatModel::SourceInletCount; ++k)
        p->values.emplace_back();
    }
    return p;
  }

  void collectInlets(
      std::shared_ptr<Ports> const& ports, int sourceCount, ossia::inlets& out) const
  {
    out.clear();
    out.push_back(const_cast<ossia::value_inlet*>(&setup_in));
    out.push_back(const_cast<ossia::value_inlet*>(&speakers_in));
    out.push_back(const_cast<ossia::value_inlet*>(&interp_in));
    out.push_back(const_cast<ossia::value_inlet*>(&cutoff_in));
    out.push_back(const_cast<ossia::value_inlet*>(&max_attenuation_in));
    for(int s = 0; s < sourceCount; ++s)
    {
      out.push_back(&ports->audio[std::size_t(s)]);
      auto const base = std::size_t(s) * (SpatModel::SourceInletCount - 1);
      for(int k = 0; k < SpatModel::SourceInletCount - 1; ++k)
        out.push_back(&ports->values[base + std::size_t(k)]);
    }
  }

  SpatNode(
      int sourceCount, int frames, std::weak_ptr<Execution::GCCommandQueue> gc,
      std::shared_ptr<LiveSources> live)
      : m_spat{frames}
      , m_gc{std::move(gc)}
      , m_live{std::move(live)}
  {
    adoptPorts(makePorts(sourceCount), sourceCount);
    m_outlets.push_back(&audio_out);
  }

  void adoptPorts(std::shared_ptr<Ports> ports, int sourceCount) noexcept
  {
    m_ports = std::move(ports);
    m_sourceCount = sourceCount;
    collectInlets(m_ports, m_sourceCount, m_inlets);
    m_spat.setSourceCount(std::size_t(sourceCount));
  }

  [[nodiscard]] std::shared_ptr<Ports> const& ports() const noexcept { return m_ports; }
  [[nodiscard]] int sourceCount() const noexcept { return m_sourceCount; }
  [[nodiscard]] int frames() const noexcept { return m_spat.frames(); }

  Prepared adopt(Prepared p) noexcept { return m_spat.adopt(std::move(p)); }

  [[nodiscard]] std::string label() const noexcept override { return "gris-spat"; }

  void run(const ossia::token_request& t, ossia::exec_state_facade f) noexcept override
  {
    readControls(f);

    auto const frames = int(f.bufferSize());
    if(frames <= 0)
      return;

    auto const channels = std::size_t(std::max(0, m_spat.numOutputChannels()));
    ossia::audio_port& out = *audio_out;
    out.set_channels(channels);
    if(channels == 0)
      return;

    auto const n = std::size_t(std::min(frames, m_spat.frames()));
    auto const numSources = std::size_t(m_sourceCount);
    for(std::size_t s = 0; s < numSources; ++s)
    {
      auto* dst = m_spat.inputBuffer(s);
      std::fill_n(dst, n, 0.f);
      ossia::audio_port const& in = *m_ports->audio[s];
      for(std::size_t c = 0; c < in.channels(); ++c)
      {
        auto const& src = in.channel(c);
        auto const m = std::min(n, src.size());
        for(std::size_t i = 0; i < m; ++i)
          dst[i] += float(src[i]);
      }
    }

    m_spat.process(int(n), GainInterpolation{m_interpolation}, m_attenuation);

    for(std::size_t c = 0; c < channels; ++c)
    {
      auto& chan = out.channel(c);
      chan.resize(std::size_t(frames));
      auto const* src = m_spat.outputBuffer(c);
      std::copy_n(src, n, chan.begin());
      std::fill(chan.begin() + n, chan.end(), 0.);

      if(c < m_live->levels.size())
      {
        float peak{};
        for(std::size_t i = 0; i < n; ++i)
          peak = std::max(peak, std::abs(src[i]));
        m_live->levels[c].store(peak, std::memory_order_relaxed);
      }
    }
  }

  ossia::value_inlet setup_in;
  ossia::value_inlet speakers_in;
  ossia::value_inlet interp_in;
  ossia::value_inlet cutoff_in;
  ossia::value_inlet max_attenuation_in;
  ossia::audio_outlet audio_out;

private:
  void readControls(ossia::exec_state_facade f)
  {
    if(auto const* v = lastValue(speakers_in))
    {
      if(*v != m_lastSpeakerList)
      {
        m_lastSpeakerList = *v;
        if(std::unique_lock lock{m_pendingMutex, std::try_to_lock})
        {
          m_pendingList = *v;
          m_hasPendingList.store(true, std::memory_order_release);
        }
        else
        {
          m_lastSpeakerList = ossia::value{};
        }
      }
    }

    if(auto const* v = lastValue(interp_in))
      m_interpolation = std::clamp(ossia::convert<float>(*v), 0.f, 1.f);

    bool attenuationChanged{};
    if(auto const* v = lastValue(cutoff_in))
    {
      m_cutoff = std::clamp(ossia::convert<float>(*v), 1.f, 24000.f);
      attenuationChanged = true;
    }
    if(auto const* v = lastValue(max_attenuation_in))
    {
      m_maxAttenuation = std::clamp(ossia::convert<float>(*v), 0.f, 1.f);
      attenuationChanged = true;
    }
    auto const rate = float(f.sampleRate());
    if(attenuationChanged || rate != m_sampleRate)
    {
      m_sampleRate = rate;
      m_attenuation.lowpassCoefficient
          = rate > 0.f ? std::exp(-ossia::two_pi * m_cutoff / rate) : 0.f;
      m_attenuation.gainAtMaxAttenuation = 1.f - m_maxAttenuation;
    }

    for(int source = 0; source < m_sourceCount; ++source)
    {
      auto const base = std::size_t(source) * (SpatModel::SourceInletCount - 1);
      auto const& ports = m_ports->values;
      if(auto const* v = lastValue(ports[base + SpatModel::Position - 1]))
      {
        auto const vec = ossia::convert<ossia::vec3f>(*v);
        m_spat.setSourcePosition(
            std::size_t(source), Position{CartesianVector{vec[0], vec[1], vec[2]}});
        auto& live = m_live->sources[std::size_t(source)];
        live.x.store(vec[0], std::memory_order_relaxed);
        live.y.store(vec[1], std::memory_order_relaxed);
        live.z.store(vec[2], std::memory_order_relaxed);
        live.placed.store(true, std::memory_order_relaxed);
      }

      auto& spans = m_spans[std::size_t(source)];
      bool spansChanged{};
      if(auto const* v = lastValue(ports[base + SpatModel::AzimuthSpan - 1]))
      {
        spans.first = ossia::convert<float>(*v);
        spansChanged = true;
      }
      if(auto const* v = lastValue(ports[base + SpatModel::ZenithSpan - 1]))
      {
        spans.second = ossia::convert<float>(*v);
        spansChanged = true;
      }
      if(spansChanged)
        m_spat.setSourceSpans(std::size_t(source), spans.first, spans.second);

      if(auto const* v = lastValue(ports[base + SpatModel::Mode - 1]))
      {
        auto const mode = ossia::convert<std::string>(*v);
        auto const mbap = mode.find("Cube") != std::string::npos;
        m_spat.setSourceMode(
            std::size_t(source), mbap ? SpatMode::mbap : SpatMode::vbap);
        m_live->sources[std::size_t(source)].mbap.store(mbap, std::memory_order_relaxed);
      }
    }
  }

  std::shared_ptr<Ports> m_ports;
  int m_sourceCount{};
  Spatializer m_spat;
  std::weak_ptr<Execution::GCCommandQueue> m_gc;
  std::shared_ptr<LiveSources> m_live;

public:
  [[nodiscard]] bool takePendingSpeakerList(ossia::value& out)
  {
    if(!m_hasPendingList.load(std::memory_order_acquire))
      return false;
    std::lock_guard lock{m_pendingMutex};
    out = std::move(m_pendingList);
    m_hasPendingList.store(false, std::memory_order_release);
    return true;
  }

private:
  std::mutex m_pendingMutex;
  ossia::value m_pendingList;
  std::atomic_bool m_hasPendingList{false};

  std::vector<std::pair<float, float>> m_spans{MAX_PROCESS_SOURCES, {0.f, 0.f}};
  ossia::value m_lastSpeakerList;
  float m_interpolation{};
  float m_cutoff{16000.f};
  float m_maxAttenuation{};
  float m_sampleRate{};
  Attenuation m_attenuation{};
};

Executor::Executor(SpatModel& proc, const Execution::Context& ctx, QObject* parent)
    : ProcessComponent_T{proc, ctx, "GrisSpatComponent", parent}
{
  auto const frames = std::max(1, ctx.execState->bufferSize);
  this->node = ossia::make_node<SpatNode>(
      *ctx.execState, proc.sourceCount(), frames, ctx.weakGCQueue(), proc.liveSources());
  m_ossia_process = std::make_shared<ossia::node_process>(this->node);

  m_oldInlets = proc.inlets();
  m_oldOutlets = proc.outlets();

  connectControls();
  useSetup(proc.layoutKey(), proc.speakerSetup());

  connect(
      &proc.speakerSetupInlet(), &Process::ControlInlet::valueChanged, this,
      [this](const ossia::value&) {
    useSetup(process().layoutKey(), process().speakerSetup());
  });
  connect(
      &proc, &SpatModel::sourceCountChanged, this, [this](int) { recomputePorts(); });
  con(ctx.doc.coarseUpdateTimer, &QTimer::timeout, this,
      [this] { applyPendingSpeakerList(); });
}

void Executor::connectControls()
{
  auto n = std::dynamic_pointer_cast<SpatNode>(this->node);
  if(!n)
    return;

  for(auto& c : m_controlConnections)
    QObject::disconnect(c);
  m_controlConnections.clear();

  auto& proc = process();
  auto const& inlets = proc.inlets();
  auto const& exec = n->root_inputs();
  auto const count = std::min(inlets.size(), exec.size());

  for(std::size_t i = 0; i < count; i++)
  {
    if(int(i) == SpatModel::SpeakerSetupPort)
      continue;

    auto* ctl = qobject_cast<Process::ControlInlet*>(inlets[i]);
    if(!ctl)
      continue;
    auto* port = dynamic_cast<ossia::value_inlet*>(exec[i]);
    if(!port)
      continue;

    ctl->setupExecution(*port, this);
    pushControlValue(port, ctl->value());

    auto c = connect(
        ctl, &Process::ControlInlet::valueChanged, this,
        [this, port](const ossia::value& v) { pushControlValue(port, v); });
    m_controlConnections.push_back(c);
  }
}

void Executor::pushControlValue(ossia::value_inlet* port, ossia::value v)
{
  std::weak_ptr<ossia::graph_node> weak = this->node;
  in_exec([port, weak, val = std::move(v)]() mutable {
    if(auto n = weak.lock())
      port->data.write_value(std::move(val), 0);
  });
}

void Executor::applyPendingSpeakerList()
{
  auto n = std::dynamic_pointer_cast<SpatNode>(this->node);
  if(!n)
    return;

  ossia::value list;
  if(!n->takePendingSpeakerList(list))
    return;

  auto setup = speakerSetupFromValue(list);
  if(!setup)
    return;

  useSetup("speakers:" + ossia::value_to_pretty_string(list), std::move(*setup));
}

Executor::~Executor() = default;

void Executor::useSetup(std::string key, SpeakerSetup setup)
{
  m_layoutKey = key;
  requestLayout(key, std::move(setup), this, [this, key](LayoutPtr layout) {
    if(key == m_layoutKey)
      sendLayout(std::move(layout));
  });
}

void Executor::sendLayout(LayoutPtr layout)
{
  auto node = std::dynamic_pointer_cast<SpatNode>(this->node);
  if(!node)
    return;
  auto payload
      = std::make_shared<Prepared>(Prepared::make(std::move(layout), node->frames()));
  std::weak_ptr<SpatNode> weak = node;
  in_exec([weak, payload, gcq = system().weakGCQueue()]() mutable {
    auto n = weak.lock();
    if(!n)
      return;
    auto old = std::make_shared<Prepared>(n->adopt(std::move(*payload)));
    payload.reset();
    if(auto q = gcq.lock())
      q->enqueue([old]() mutable { old.reset(); });
  });
}

void Executor::recomputePorts()
{
  auto n = std::dynamic_pointer_cast<SpatNode>(this->node);
  if(!n)
    return;

  auto& element = process();
  const Execution::Context& ctx = system();
  Execution::SetupContext& setup = ctx.setup;

  const Process::Inlets old_inlets = std::move(m_oldInlets);
  const Process::Outlets old_outlets = std::move(m_oldOutlets);
  const Process::Inlets& new_inlets = element.inlets();
  const Process::Outlets& new_outlets = element.outlets();

  ossia::small_vector<Process::Cable*, 8> cables;
  auto collect_cables = [&](const auto& ports) {
    for(auto port : ports)
      for(auto& cbl : port->cables())
        if(auto c = cbl.try_find(ctx.doc))
          if(setup.m_cables.find(c->id()) != setup.m_cables.end())
            if(!ossia::contains(cables, c))
              cables.push_back(c);
  };
  collect_cables(old_inlets);
  collect_cables(old_outlets);
  collect_cables(new_inlets);
  collect_cables(new_outlets);

  auto const count = element.sourceCount();
  auto ports = SpatNode::makePorts(count);
  auto inbuf = std::make_shared<ossia::inlets>();
  n->collectInlets(ports, count, *inbuf);
  SCORE_SOFT_ASSERT(inbuf->size() == new_inlets.size());

  Execution::Transaction commands{ctx};

  for(auto c : cables)
    setup.removeCable(*c, commands);

  setup.unregister_node_soft(old_inlets, old_outlets, this->node, commands);

  commands.push_back([node = n, ports, count, inbuf, gcq = ctx.weakGCQueue(),
                      wg = std::weak_ptr{ctx.execGraph}]() mutable {
    if(auto g = wg.lock())
    {
      for(auto* p : node->root_inputs())
      {
        if(ossia::contains(*inbuf, p))
          continue;
        const auto edges = p->sources;
        for(auto e : edges)
          g->disconnect(e);
      }
    }
    auto old = node->ports();
    node->adoptPorts(std::move(ports), count);
    if(auto q = gcq.lock())
      q->enqueue([old, inbuf]() mutable {
        old.reset();
        inbuf.reset();
      });
  });

  const std::size_t n_in = std::min(new_inlets.size(), inbuf->size());
  for(std::size_t i = 0; i < n_in; i++)
    setup.register_inlet(*new_inlets[i], (*inbuf)[i], this->node, commands);
  if(!new_outlets.empty())
    setup.register_outlet(*new_outlets[0], &n->audio_out, this->node, commands);

  for(auto c : cables)
    setup.connectCable(*c, commands);
  this->portsReplaced(&commands);

  commands.run_all();

  m_oldInlets = new_inlets;
  m_oldOutlets = new_outlets;

  connectControls();
}
}
