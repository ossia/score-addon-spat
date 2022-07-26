#pragma once

#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/meta.hpp>

namespace Spat
{

class StereoToMono
{
public:
  halp_meta(name, "Stereo To Mono")
  halp_meta(category, "Audio")
  halp_meta(c_name, "avnd_stereotomono")
  halp_meta(uuid, "82bdb9c5-9cf1-440e-8675-c0caf4fc59b9")

  using setup = halp::setup;
  using tick = halp::tick;

  struct ins
  {
    halp::fixed_audio_bus<"Input", double, 2> audio;
  } inputs;

  struct
  {
    halp::dynamic_audio_bus<"Output", double> audio;
  } outputs;

  struct ui;

  void operator()(halp::tick tick);
};
}
