#include <Process/Dataflow/Port.hpp>
#include <Process/Dataflow/WidgetInlets.hpp>

#include <score/model/Identifier.hpp>
#include <score/serialization/VisitorCommon.hpp>

#include <ossia/network/value/value_conversion.hpp>

#include <QWidget>

#include <Gris/Algo/SpeakerSetupIO.hpp>
#include <Gris/LayoutCache.hpp>
#include <Gris/LiveSources.hpp>
#include <Gris/Model.hpp>

#include <wobjectimpl.h>

W_OBJECT_IMPL(Gris::SpatModel)

namespace Gris
{
SpatModel::SpatModel(
    const TimeVal& duration, const Id<Process::ProcessModel>& id, QObject* parent)
    : Process::ProcessModel{duration, id, "GrisSpat", parent}
{
  m_inlets.push_back(
      new SpeakerSetupInlet{tr("Speaker setup"), Id<Process::Port>(0), this});
  m_inlets.push_back(
      new Process::ValueInlet{tr("Speakers"), Id<Process::Port>(1), this});
  m_inlets.push_back(new Process::FloatSlider{
      0.f, 1.f, 0.f, tr("Interpolation"), Id<Process::Port>(2), this});
  m_inlets.push_back(new Process::LogFloatSlider{
      20.f, 20000.f, 16000.f, tr("Attenuation cutoff"), Id<Process::Port>(3), this});
  m_inlets.push_back(new Process::FloatSlider{
      0.f, 1.f, 0.f, tr("Max attenuation"), Id<Process::Port>(4), this});
  m_outlets.push_back(
      new Process::AudioOutlet{tr("Speakers"), Id<Process::Port>(0), this});
  safe_cast<Process::AudioOutlet*>(m_outlets.back())->setPropagate(true);

  int nextId = FixedInletCount;
  for(int source = 0; source < m_sourceCount; ++source)
    addSourcePorts(source, nextId);

  metadata().setInstanceName(*this);
  init();
}

SpatModel::~SpatModel()
{
  if(externalUI)
    externalUI->close();
}

void SpatModel::init()
{
  m_live = std::make_shared<LiveSources>();

  if(foldMode() == Process::FoldMode::Auto)
    setFoldMode(Process::FoldMode::Unfolded);

  for(auto* inlet : m_inlets)
    inlet->displayHandledExplicitly = true;
  for(auto* outlet : m_outlets)
    outlet->displayHandledExplicitly = true;

  connect(
      &speakerSetupInlet(), &Process::ControlInlet::valueChanged, this,
      [this](const ossia::value&) { prepareLayout(); });
  prepareLayout();
}

void SpatModel::prepareLayout()
{
  auto key = layoutKey();
  requestLayout(key, speakerSetup(), this, [this, key](LayoutPtr layout) {
    if(key == layoutKey())
      m_layout = std::move(layout);
  });
}

SpeakerSetupInlet& SpatModel::speakerSetupInlet() const noexcept
{
  return *safe_cast<SpeakerSetupInlet*>(m_inlets[SpeakerSetupPort]);
}

SpeakerSetup SpatModel::speakerSetup() const noexcept
{
  return speakerSetupInlet().setup();
}

std::string SpatModel::layoutKey() const
{
  return ossia::convert<std::string>(speakerSetupInlet().value());
}

void SpatModel::setSourceCount(int count)
{
  count = std::clamp(count, 1, maxSourceCount);
  if(count == m_sourceCount)
    return;

  int const previous = m_sourceCount;
  m_sourceCount = count;

  std::vector<Process::Inlet*> removed;
  if(count > previous)
  {
    int nextId{};
    for(auto* inlet : m_inlets)
      nextId = std::max(nextId, inlet->id().val() + 1);
    for(int source = previous; source < count; ++source)
      addSourcePorts(source, nextId);
  }
  else
  {
    auto const keep = std::size_t(FixedInletCount + count * SourceInletCount);
    removed.assign(m_inlets.begin() + keep, m_inlets.end());
    m_inlets.resize(keep);
  }

  sourceCountChanged(count);
  inletsChanged();

  for(auto* port : removed)
    delete port;
}

void SpatModel::addSourcePorts(int source, int& nextId)
{
  auto const label = QString::number(source + 1);

  m_inlets.push_back(new Process::AudioInlet{
      tr("Source %1").arg(label), Id<Process::Port>(nextId++), this});
  m_inlets.push_back(new Process::XYZSlider{
      ossia::vec3f{-2.f, -2.f, -2.f}, ossia::vec3f{2.f, 2.f, 2.f},
      ossia::vec3f{0.f, 0.f, 0.f}, tr("Source %1 position").arg(label),
      Id<Process::Port>(nextId++), this});
  m_inlets.push_back(new Process::FloatSlider{
      0.f, 1.f, 0.f, tr("Source %1 azimuth span").arg(label),
      Id<Process::Port>(nextId++), this});
  m_inlets.push_back(new Process::FloatSlider{
      0.f, 1.f, 0.f, tr("Source %1 zenith span").arg(label), Id<Process::Port>(nextId++),
      this});
  m_inlets.push_back(new Process::Enum{
      std::vector<std::string>{"Dome (VBAP)", "Cube (MBAP)"}, std::vector<QString>{},
      "Dome (VBAP)", tr("Source %1 mode").arg(label), Id<Process::Port>(nextId++),
      this});

  for(auto i = m_inlets.size() - SourceInletCount; i < m_inlets.size(); ++i)
    m_inlets[i]->displayHandledExplicitly = true;
}
}
