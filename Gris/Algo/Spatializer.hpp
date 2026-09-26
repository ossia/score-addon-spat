#pragma once

#include <Gris/Algo/GainMatrix.hpp>
#include <Gris/Algo/Mbap.hpp>
#include <Gris/Algo/SpeakerSetup.hpp>
#include <Gris/Algo/Types.hpp>
#include <Gris/Algo/Vbap.hpp>

#include <memory>
#include <vector>

namespace Gris
{
inline constexpr std::size_t MAX_PROCESS_SOURCES = 128;

struct SourceState
{
  SourceData data{};
  SpatMode mode{SpatMode::vbap};
  bool dirty{true};
  float attenuationDistance{};
  float attenuationGain{1.f};
  float attenuationCoefficient{};
  float lowpassY{};
  float lowpassZ{};
};

struct Attenuation
{
  float lowpassCoefficient{};
  float gainAtMaxAttenuation{1.f};
};

struct Layout
{
  SpeakerSetup setup{};
  SpeakersData flat{};
  std::vector<int> channelOfSpeaker{};
  int numOutputChannels{};
  std::unique_ptr<VbapData> vbap{};
  MbapField mbap{};
  bool mbapUsable{};

  [[nodiscard]] static std::shared_ptr<Layout const> make(SpeakerSetup setup);
};

struct Prepared
{
  std::shared_ptr<Layout const> layout{};
  GainMatrix target{};
  GainMatrix last{};
  std::vector<float*> speakerPtrs{};
  std::vector<std::vector<float>> outScratch{};
  std::vector<float*> outPtrs{};

  [[nodiscard]] static Prepared make(std::shared_ptr<Layout const> layout, int frames);
};

class Spatializer
{
public:
  explicit Spatializer(int frames);
  ~Spatializer();

  Prepared adopt(Prepared next) noexcept;

  [[nodiscard]] Layout const* layout() const noexcept { return m_prepared.layout.get(); }
  [[nodiscard]] int numOutputChannels() const noexcept
  {
    return m_prepared.layout ? m_prepared.layout->numOutputChannels : 0;
  }
  [[nodiscard]] int frames() const noexcept { return m_frames; }
  [[nodiscard]] bool vbapUsable() const noexcept
  {
    return m_prepared.layout && m_prepared.layout->vbap != nullptr;
  }
  [[nodiscard]] bool mbapUsable() const noexcept
  {
    return m_prepared.layout && m_prepared.layout->mbapUsable;
  }

  void setSourceCount(std::size_t count) noexcept;
  [[nodiscard]] std::size_t sourceCount() const noexcept { return m_sourceCount; }

  void setSourcePosition(std::size_t source, Position const& position) noexcept;
  void setSourceSpans(std::size_t source, float azimuthSpan, float zenithSpan) noexcept;
  void setSourceMode(std::size_t source, SpatMode mode) noexcept;
  void clearSourcePosition(std::size_t source) noexcept;

  [[nodiscard]] float* inputBuffer(std::size_t source) noexcept
  {
    return m_inScratch[source].data();
  }
  [[nodiscard]] float const* outputBuffer(std::size_t channel) const noexcept
  {
    return m_prepared.outScratch[channel].data();
  }

  void updateGains() noexcept;

  void process(int numSamples, GainInterpolation interp, Attenuation atten) noexcept;

  void process(
      float const* const* inputs, std::size_t numInputs, float* const* outputs,
      std::size_t numOutputs, int numSamples, GainInterpolation interp,
      Attenuation atten = {}) noexcept;

  [[nodiscard]] std::vector<Triplet> triplets() const;

private:
  int m_frames{};
  std::size_t m_sourceCount{};
  std::vector<SourceState> m_sources{};
  std::vector<std::vector<float>> m_inScratch{};
  Prepared m_prepared{};
  SpeakersSpatGains m_scratch{};
};

}
