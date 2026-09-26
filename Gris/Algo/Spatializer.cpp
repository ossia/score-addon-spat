#include <Gris/Algo/Spatializer.hpp>

#include <cmath>

#include <algorithm>

namespace Gris
{
namespace
{
constexpr float NORMAL_RADIUS = 1.f;
constexpr float EXTENDED_RADIUS = 1.6666667f;
constexpr float EXTRA_DISTANCE = EXTENDED_RADIUS - NORMAL_RADIUS;

float attenuationDistance(CartesianVector const& c) noexcept
{
  auto const distXY = std::sqrt(c.x * c.x + c.y * c.y);
  if(c.z < 0.f && distXY < NORMAL_RADIUS)
    return std::abs(c.z - NORMAL_RADIUS);
  if(c.z < 0.f)
    return distXY + std::abs(c.z);
  return std::sqrt(c.x * c.x + c.y * c.y + c.z * c.z);
}

void applyAttenuation(
    SourceState& s, float* data, int numSamples, Attenuation const& a) noexcept
{
  auto const ratio
      = std::clamp((s.attenuationDistance - NORMAL_RADIUS) / EXTRA_DISTANCE, 0.f, 1.f);
  auto const targetGain = 1.f - ratio * (1.f - a.gainAtMaxAttenuation);
  auto const targetCoefficient = ratio * a.lowpassCoefficient;
  auto const gainStep = (targetGain - s.attenuationGain) / float(numSamples);
  auto const coefficientStep
      = (targetCoefficient - s.attenuationCoefficient) / float(numSamples);

  if(gainStep == 0.f && coefficientStep == 0.f)
  {
    if(ratio == 0.f)
      return;
    for(int i = 0; i < numSamples; ++i)
    {
      s.lowpassY = data[i] + (s.lowpassY - data[i]) * s.attenuationCoefficient;
      s.lowpassZ = s.lowpassY + (s.lowpassZ - s.lowpassY) * s.attenuationCoefficient;
      data[i] = s.lowpassZ * s.attenuationGain;
    }
    return;
  }

  for(int i = 0; i < numSamples; ++i)
  {
    s.attenuationCoefficient += coefficientStep;
    s.attenuationGain += gainStep;
    s.lowpassY = data[i] + (s.lowpassY - data[i]) * s.attenuationCoefficient;
    s.lowpassZ = s.lowpassY + (s.lowpassZ - s.lowpassY) * s.attenuationCoefficient;
    data[i] = s.lowpassZ * s.attenuationGain;
  }
}
}

std::shared_ptr<Layout const> Layout::make(SpeakerSetup setup)
{
  auto layout = std::make_shared<Layout>();
  layout->setup = std::move(setup);
  layout->flat = layout->setup.flattened();
  layout->numOutputChannels = layout->setup.maxOutputPatch();

  auto const& flat = layout->flat;
  layout->channelOfSpeaker.assign(flat.size(), -1);
  for(std::size_t i = 0; i < flat.size(); ++i)
  {
    if(flat[i].data.isDirectOutOnly || flat[i].data.state == SliceState::muted)
      continue;
    layout->channelOfSpeaker[i] = flat[i].patch.get() - 1;
  }

  {
    std::array<Position, MAX_NUM_SPEAKERS> positions{};
    std::array<output_patch_t, MAX_NUM_SPEAKERS> patches{};
    int count{};
    bool is3d{};
    for(auto const& speaker : flat)
    {
      if(speaker.data.isDirectOutOnly || count >= MAX_NUM_SPEAKERS)
        continue;
      positions[static_cast<std::size_t>(count)] = speaker.data.position;
      patches[static_cast<std::size_t>(count)] = speaker.patch;
      if(std::abs(speaker.data.position.getPolar().elevation.get()) > 0.001f)
        is3d = true;
      ++count;
    }
    if(count >= 3)
      layout->vbap = vbapInit(positions, count, is3d ? 3 : 2, patches);
  }

  {
    SpeakersData spatialized;
    for(auto const& speaker : flat)
      if(!speaker.data.isDirectOutOnly)
        spatialized.push_back(speaker);

    if(spatialized.size() >= 2)
    {
      layout->mbap = mbapInit(spatialized);
      constexpr float DIFFUSION_IN_MIN{1.f};
      constexpr float DIFFUSION_IN_MAX{0.f};
      constexpr float DIFFUSION_OUT_MIN{1.f};
      constexpr float DIFFUSION_OUT_MAX{8.f};
      layout->mbap.fieldExponent = ((layout->setup.diffusion - DIFFUSION_IN_MIN)
                                    * (DIFFUSION_OUT_MAX - DIFFUSION_OUT_MIN)
                                    / (DIFFUSION_IN_MAX - DIFFUSION_IN_MIN))
                                   + DIFFUSION_OUT_MIN;
      layout->mbapUsable = true;
    }
  }

  return layout;
}

Prepared Prepared::make(std::shared_ptr<Layout const> layout, int frames)
{
  Prepared p;
  auto const numSpeakers = layout ? layout->flat.size() : 0;
  auto const numChannels
      = layout ? std::size_t(std::max(0, layout->numOutputChannels)) : 0;
  p.layout = std::move(layout);
  p.target.resize(MAX_PROCESS_SOURCES, numSpeakers);
  p.last.resize(MAX_PROCESS_SOURCES, numSpeakers);
  p.speakerPtrs.assign(numSpeakers, nullptr);
  p.outScratch.assign(
      numChannels, std::vector<float>(std::size_t(std::max(0, frames)), 0.f));
  p.outPtrs.resize(numChannels);
  for(std::size_t c = 0; c < numChannels; ++c)
    p.outPtrs[c] = p.outScratch[c].data();
  return p;
}

Spatializer::Spatializer(int frames)
    : m_frames{std::max(1, frames)}
    , m_sources(MAX_PROCESS_SOURCES)
    , m_inScratch(MAX_PROCESS_SOURCES, std::vector<float>(std::size_t(m_frames), 0.f))
{
}

Spatializer::~Spatializer() = default;

Prepared Spatializer::adopt(Prepared next) noexcept
{
  std::swap(m_prepared, next);
  if(m_prepared.last.numSources() == next.last.numSources()
     && m_prepared.last.numSpeakers() == next.last.numSpeakers())
    std::swap(m_prepared.last, next.last);
  for(auto& s : m_sources)
    s.dirty = true;
  return next;
}

void Spatializer::setSourceCount(std::size_t count) noexcept
{
  m_sourceCount = std::min(count, MAX_PROCESS_SOURCES);
}

void Spatializer::setSourcePosition(
    std::size_t source, Position const& position) noexcept
{
  if(source >= m_sources.size())
    return;
  auto& s = m_sources[source];
  if(s.data.position && *s.data.position == position)
    return;
  s.data.position = position;
  s.attenuationDistance = attenuationDistance(position.getCartesian());
  s.dirty = true;
}

void Spatializer::setSourceSpans(
    std::size_t source, float azimuthSpan, float zenithSpan) noexcept
{
  if(source >= m_sources.size())
    return;
  auto& s = m_sources[source];
  if(s.data.azimuthSpan == azimuthSpan && s.data.zenithSpan == zenithSpan)
    return;
  s.data.azimuthSpan = azimuthSpan;
  s.data.zenithSpan = zenithSpan;
  s.dirty = true;
}

void Spatializer::setSourceMode(std::size_t source, SpatMode mode) noexcept
{
  if(source >= m_sources.size())
    return;
  auto& s = m_sources[source];
  auto const resolved = (mode == SpatMode::mbap) ? SpatMode::mbap : SpatMode::vbap;
  if(s.mode == resolved)
    return;
  s.mode = resolved;
  s.dirty = true;
}

void Spatializer::clearSourcePosition(std::size_t source) noexcept
{
  if(source >= m_sources.size())
    return;
  auto& s = m_sources[source];
  if(!s.data.position)
    return;
  s.data.position.reset();
  s.dirty = true;
}

void Spatializer::updateGains() noexcept
{
  auto const* L = m_prepared.layout.get();
  if(!L || L->flat.empty())
    return;

  for(std::size_t src = 0; src < m_sourceCount; ++src)
  {
    auto& state = m_sources[src];
    if(!state.dirty)
      continue;

    auto* row = m_prepared.target.row(src);
    std::fill_n(row, L->flat.size(), 0.f);

    if(state.data.position)
    {
      m_scratch.fill(0.f);
      if(state.mode == SpatMode::mbap && L->mbapUsable)
        mbap(state.data, m_scratch, L->mbap);
      else if(L->vbap)
        vbapCompute(state.data, m_scratch, *L->vbap);
      for(std::size_t i = 0; i < L->flat.size(); ++i)
        row[i] = m_scratch[L->flat[i].patch];
    }

    state.dirty = false;
  }
}

void Spatializer::process(
    int numSamples, GainInterpolation interp, Attenuation atten) noexcept
{
  auto const* L = m_prepared.layout.get();
  if(!L || L->flat.empty() || numSamples <= 0)
    return;
  numSamples = std::min(numSamples, m_frames);

  auto const numChannels = m_prepared.outScratch.size();
  for(auto& chan : m_prepared.outScratch)
    std::fill_n(chan.data(), std::min(std::size_t(numSamples), chan.size()), 0.f);

  for(std::size_t i = 0; i < L->flat.size(); ++i)
  {
    auto const channel = L->channelOfSpeaker[i];
    m_prepared.speakerPtrs[i]
        = (channel >= 0 && static_cast<std::size_t>(channel) < numChannels)
              ? m_prepared.outPtrs[static_cast<std::size_t>(channel)]
              : nullptr;
  }

  updateGains();

  for(std::size_t src = 0; src < m_sourceCount; ++src)
  {
    auto& state = m_sources[src];
    auto* input = m_inScratch[src].data();
    if(state.data.position && state.mode == SpatMode::mbap && L->mbapUsable)
      applyAttenuation(state, input, numSamples, atten);

    applySourceGains(
        m_prepared.target.row(src), m_prepared.last.row(src), input,
        m_prepared.speakerPtrs.data(), L->flat.size(), numSamples, interp);
  }
}

void Spatializer::process(
    float const* const* inputs, std::size_t numInputs, float* const* outputs,
    std::size_t numOutputs, int numSamples, GainInterpolation interp,
    Attenuation atten) noexcept
{
  numSamples = std::min(numSamples, m_frames);
  auto const numSources = std::min({numInputs, m_sourceCount, m_sources.size()});
  for(std::size_t s = 0; s < numSources; ++s)
  {
    auto* dst = m_inScratch[s].data();
    if(inputs[s])
      std::copy_n(inputs[s], numSamples, dst);
    else
      std::fill_n(dst, numSamples, 0.f);
  }

  process(numSamples, interp, atten);

  auto const numChannels = std::min(numOutputs, m_prepared.outScratch.size());
  for(std::size_t c = 0; c < numChannels; ++c)
  {
    if(!outputs[c])
      continue;
    auto const* src = m_prepared.outScratch[c].data();
    for(int i = 0; i < numSamples; ++i)
      outputs[c][i] += src[i];
  }
}

std::vector<Triplet> Spatializer::triplets() const
{
  auto const* L = m_prepared.layout.get();
  if(!L || !L->vbap || L->vbap->dimension != 3)
    return {};
  return vbapExtractTriplets(*L->vbap);
}

}
