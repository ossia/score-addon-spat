#pragma once

#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/meta.hpp>

#include <iostream>

namespace Spat
{

class StereoPanning
{
public:
  halp_meta(name, "Stereo Panning")
  halp_meta(category, "Audio")
  halp_meta(c_name, "avnd_stereopanning")
  halp_meta(uuid, "82bdb9c5-9cf8-440e-8675-c0caf4fc59b9")

  using setup = halp::setup;
  using tick = halp::tick;

  struct ins
  {
    halp::fixed_audio_bus<"Input", double, 2> audio;
    halp::hslider_f32<"L/R", halp::range{.min = -1, .max = 1, .init = 0}> lr;
  } inputs;

  struct
  {
    halp::dynamic_audio_bus<"Output", double> audio;
  } outputs;

  struct ui;

  void operator()(halp::tick tick);
};
}
