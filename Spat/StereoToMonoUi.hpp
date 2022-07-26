#pragma once
#include <Spat/StereoToMonoModel.hpp>
#include <halp/layout.hpp>

namespace Spat
{
struct StereoToMono::ui
{
  using enum halp::colors;
  using enum halp::layouts;

  halp_meta(name, "Main")
  halp_meta(layout, vbox)
  halp_meta(background, dark)

  halp::label title{"Stereo To Mono"};
};
}
