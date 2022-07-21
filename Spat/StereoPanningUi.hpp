#pragma once
#include <Spat/StereoPanningModel.hpp>
#include <halp/layout.hpp>

namespace Spat
{
struct StereoPanning::ui
{
  using enum halp::colors;
  using enum halp::layouts;

  halp_meta(name, "Main") halp_meta(layout, vbox) halp_meta(background, dark)

      halp::label title{"Stereo Panning"};
  halp::item<&ins::lr> lr;
};
}
