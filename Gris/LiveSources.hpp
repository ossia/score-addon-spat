#pragma once

#include <Gris/Algo/Spatializer.hpp>

#include <array>
#include <atomic>

namespace Gris
{
struct LiveSource
{
  std::atomic<float> x{};
  std::atomic<float> y{};
  std::atomic<float> z{};
  std::atomic<bool> mbap{};
  std::atomic<bool> placed{};
};

struct LiveSources
{
  std::array<LiveSource, MAX_PROCESS_SOURCES> sources{};
  std::array<std::atomic<float>, MAX_NUM_SPEAKERS> levels{};
};
}
