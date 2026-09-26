#include <ossia/network/value/value_conversion.hpp>

#include <Gris/SpeakerList.hpp>

#include <cmath>

#include <algorithm>

namespace Gris
{
namespace
{
bool isNumber(ossia::value const& v) noexcept
{
  switch(v.get_type())
  {
    case ossia::val_type::FLOAT:
    case ossia::val_type::INT:
    case ossia::val_type::BOOL:
      return true;
    default:
      return false;
  }
}

struct ParsedSpeaker
{
  CartesianVector position{};
  bool enabled{true};
};

std::optional<ParsedSpeaker> parseSpeaker(ossia::value const& v)
{
  ParsedSpeaker s;
  switch(v.get_type())
  {
    case ossia::val_type::VEC3F: {
      auto const& p = *v.target<ossia::vec3f>();
      s.position = {p[0], p[1], p[2]};
      return s;
    }
    case ossia::val_type::LIST: {
      auto const& l = *v.target<std::vector<ossia::value>>();
      if(l.size() >= 3 && isNumber(l[0]) && isNumber(l[1]) && isNumber(l[2]))
      {
        s.position
            = {ossia::convert<float>(l[0]), ossia::convert<float>(l[1]),
               ossia::convert<float>(l[2])};
        return s;
      }
      if(l.size() >= 4 && l[0].get_type() == ossia::val_type::STRING && isNumber(l[1])
         && isNumber(l[2]) && isNumber(l[3]))
      {
        s.position
            = {ossia::convert<float>(l[1]), ossia::convert<float>(l[2]),
               ossia::convert<float>(l[3])};
        if(l.size() >= 11 && l[10].get_type() == ossia::val_type::BOOL)
          s.enabled = *l[10].target<bool>();
        return s;
      }
      return std::nullopt;
    }
    default:
      return std::nullopt;
  }
}
}

std::optional<SpeakerSetup> speakerSetupFromValue(ossia::value const& v)
{
  if(v.get_type() != ossia::val_type::LIST)
    return std::nullopt;
  auto const& items = *v.target<std::vector<ossia::value>>();
  if(items.empty())
    return std::nullopt;

  std::vector<ParsedSpeaker> speakers;
  speakers.reserve(items.size());
  for(auto const& item : items)
  {
    auto s = parseSpeaker(item);
    if(!s)
      return std::nullopt;
    speakers.push_back(*s);
  }

  float maxAbs{};
  for(auto const& s : speakers)
    maxAbs = std::max(
        {maxAbs, std::abs(s.position.x), std::abs(s.position.y),
         std::abs(s.position.z)});
  float const scale = maxAbs > 1.7f ? 1.f / maxAbs : 1.f;

  SpeakerSetup setup;
  setup.spatMode = SpatMode::hybrid;
  SpeakerGroup group;
  int patch = 1;
  for(auto const& s : speakers)
  {
    SpeakerEntry e;
    e.patch = output_patch_t{patch++};
    e.data.position = Position{CartesianVector{
        s.position.x * scale, s.position.y * scale, s.position.z * scale}};
    e.data.state = s.enabled ? SliceState::normal : SliceState::muted;
    group.speakers.push_back(e);
  }
  setup.groups.push_back(std::move(group));
  return setup;
}
}
