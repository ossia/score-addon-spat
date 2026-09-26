#pragma once

#include <ossia/network/value/value.hpp>

#include <Gris/Algo/SpeakerSetup.hpp>

#include <optional>

namespace Gris
{
[[nodiscard]] std::optional<SpeakerSetup> speakerSetupFromValue(ossia::value const& v);
}
