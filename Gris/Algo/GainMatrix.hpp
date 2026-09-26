#pragma once

#include <Gris/Algo/Types.hpp>

#include <cmath>

#include <cstddef>
#include <vector>

namespace Gris
{
class GainMatrix
{
public:
  void resize(std::size_t numSources, std::size_t numSpeakers)
  {
    m_numSources = numSources;
    m_numSpeakers = numSpeakers;
    m_gains.assign(numSources * numSpeakers, 0.f);
  }

  void clear() noexcept
  {
    for(auto& g : m_gains)
      g = 0.f;
  }

  [[nodiscard]] float* row(std::size_t source) noexcept
  {
    return m_gains.data() + source * m_numSpeakers;
  }
  [[nodiscard]] float const* row(std::size_t source) const noexcept
  {
    return m_gains.data() + source * m_numSpeakers;
  }

  [[nodiscard]] std::size_t numSources() const noexcept { return m_numSources; }
  [[nodiscard]] std::size_t numSpeakers() const noexcept { return m_numSpeakers; }
  [[nodiscard]] bool empty() const noexcept { return m_gains.empty(); }

private:
  std::vector<float> m_gains{};
  std::size_t m_numSources{};
  std::size_t m_numSpeakers{};
};

struct GainInterpolation
{
  float amount{};

  [[nodiscard]] float factor() const noexcept
  {
    return std::pow(amount, 0.1f) * 0.0099f + 0.99f;
  }
};

inline void applySourceGains(
    float const* target, float* last, float const* input, float* const* outputs,
    std::size_t numSpeakers, int numSamples, GainInterpolation interp) noexcept
{
  auto const gainFactor = interp.factor();

  for(std::size_t spk = 0; spk < numSpeakers; ++spk)
  {
    auto* outputSamples = outputs[spk];
    if(outputSamples == nullptr)
      continue;

    auto& currentGain = last[spk];
    auto const targetGain = target[spk];
    auto const gainDiff = targetGain - currentGain;
    auto const gainSlope = gainDiff / static_cast<float>(numSamples);

    if(gainSlope == 0.f || std::abs(gainDiff) < SMALL_GAIN)
    {
      currentGain = targetGain;
      if(currentGain >= SMALL_GAIN)
        for(int i = 0; i < numSamples; ++i)
          outputSamples[i] += input[i] * currentGain;
      continue;
    }

    if(interp.amount == 0.f)
    {
      for(int i = 0; i < numSamples; ++i)
      {
        currentGain += gainSlope;
        outputSamples[i] += input[i] * currentGain;
      }
    }
    else if(targetGain < SMALL_GAIN)
    {
      for(int i = 0; i < numSamples && currentGain >= SMALL_GAIN; ++i)
      {
        currentGain = targetGain + (currentGain - targetGain) * gainFactor;
        outputSamples[i] += input[i] * currentGain;
      }
    }
    else
    {
      for(int i = 0; i < numSamples; ++i)
      {
        currentGain = targetGain + (currentGain - targetGain) * gainFactor;
        outputSamples[i] += input[i] * currentGain;
      }
    }
  }
}

}
