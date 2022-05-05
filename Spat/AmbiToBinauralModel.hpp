#pragma once

#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/meta.hpp>

namespace Spat
{

class AmbiToBinaural
{
public:
  halp_meta(name, "Ambisonics to Binaural")
  halp_meta(category, "Audio")
  halp_meta(c_name, "ambi_to_binaural")
  halp_meta(uuid, "0fb2b7f5-8811-472a-abad-905e1fa0e6db")

  struct ins
  {
    halp::dynamic_audio_bus<"Input", double> audio;
    halp::knob_f32<"Weight", halp::range{.min = 0., .max = 10., .init = 0.5}> gain;
  } inputs;

  struct
  {
    halp::dynamic_audio_bus<"Output", double> audio;
  } outputs;

  using setup = halp::setup;
  void prepare(halp::setup info)
  {
  }

  using tick = halp::tick;

  void operator()(halp::tick t);

  struct ui;
};

}
