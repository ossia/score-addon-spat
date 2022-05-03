#pragma once
#include <Spat/RotatorModel.hpp>
#include <halp/layout.hpp>

namespace Spat
{
struct Rotator::ui
{
  using enum halp::colors;
  using enum halp::layouts;

  halp_meta(name, "Main")
  halp_meta(layout, vbox)
  halp_meta(background, dark)

  halp::label title{"Rotator (with Spherical Harmonics / Ambisonics)"};
  halp::item<&ins::order> order;
  halp::item<&ins::conv> conv;
  halp::item<&ins::yaw> yaw;
  halp::item<&ins::pitch> pitch;
  halp::item<&ins::roll> roll;

};
}
