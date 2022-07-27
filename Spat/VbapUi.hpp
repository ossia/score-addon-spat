#pragma once
#include <Spat/VbapModel.hpp>
#include <halp/layout.hpp>

namespace Spat
{
struct Vbap::ui
{
  using enum halp::colors;
  using enum halp::layouts;

  halp_meta(name, "Main")
  halp_meta(layout, vbox)
  halp_meta(background, dark)

  halp::label title{"VBAP"};
  halp::item<&ins::room> room;
  halp::item<&ins::azi> azi;
};
}
