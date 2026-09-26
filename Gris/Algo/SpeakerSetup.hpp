#pragma once

#include <Gris/Algo/Types.hpp>

#include <string>
#include <vector>

namespace Gris
{
enum class SpatMode : std::int8_t
{
  invalid = -1,
  vbap = 0,
  mbap,
  hybrid
};

[[nodiscard]] std::string_view toString(SpatMode) noexcept;
[[nodiscard]] SpatMode spatModeFromString(std::string_view) noexcept;

enum class SliceState : std::uint8_t
{
  normal = 0,
  muted,
  solo
};

[[nodiscard]] std::string_view toString(SliceState) noexcept;
[[nodiscard]] SliceState sliceStateFromString(std::string_view) noexcept;

struct SpeakerHighpass
{
  float freq{};
};

struct SpeakerData
{
  Position position{};
  SliceState state{SliceState::normal};
  float gain{};
  float highpassFreq{};
  bool isDirectOutOnly{};
};

struct SpeakerEntry
{
  output_patch_t patch{};
  SpeakerData data{};
};

using SpeakersData = std::vector<SpeakerEntry>;

struct SpeakerGroup
{
  std::string name{};
  CartesianVector position{};
  degrees_t yaw{};
  degrees_t pitch{};
  degrees_t roll{};
  std::vector<SpeakerEntry> speakers{};
};

struct SpeakerSetup
{
  SpatMode spatMode{SpatMode::vbap};
  float diffusion{};
  bool generalMute{};
  std::vector<SpeakerGroup> groups{};

  [[nodiscard]] SpeakersData flattened() const;

  [[nodiscard]] int numSpatializedSpeakers() const noexcept;

  [[nodiscard]] int maxOutputPatch() const noexcept;

  [[nodiscard]] bool isDomeLike() const noexcept;
};

}
