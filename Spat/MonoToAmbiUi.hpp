#pragma once
#include <Spat/MonoToAmbiModel.hpp>
#include <halp/layout.hpp>

namespace Spat
{
struct MonoToAmbi::ui
{
  using enum halp::colors;
  using enum halp::layouts;

  halp_meta(name, "Main")
  halp_meta(layout, vbox)
  halp_meta(background, dark)

  halp::label title{"Places a mono signal in space (with Spherical Harmonics / Ambisonics)"};
  halp::item<&ins::order> order;
  halp::item<&ins::conv> conv;
  halp::item<&ins::azi> azi;
  halp::item<&ins::elev> elev;
};
}
