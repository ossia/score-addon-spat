#pragma once

#include <Spat/_AmbisonicMethods.hpp>
#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/fft.hpp>
#include <halp/meta.hpp>

#include <complex>
#include <iostream>

namespace Spat
{

class AmbiToBinaural
{
  static constexpr int max_order = 7;
  static constexpr int max_nsh = ((max_order + 1) * (max_order + 1));
  static constexpr float deg_to_rad = M_PI / 180.f;

public:
  halp_meta(name, "Ambisonics to Binaural")
  halp_meta(category, "Audio")
  halp_meta(c_name, "ambi_to_binaural")
  halp_meta(uuid, "0fb2b7f5-8811-472a-abad-905e1fa0e6db")

  struct ins
  {
    //halp::dynamic_audio_bus<"Input", float>
    //    audio;
    halp::dynamic_audio_spectrum_bus<"Input", float> audio;
    halp::knob_f32<"Azimuth", halp::range{.min = -180.0, .max = 180.0, .init = 0}> azi;
    halp::knob_f32<"Elevation", halp::range{.min = -180.0, .max = 180.0, .init = 0}>
        elev;
    halp::hslider_i32<"Order", halp::range{.min = 0, .max = max_order, .init = 0}> order;
    halp::knob_f32<"Yaw", halp::range{.min = -180.0, .max = 180.0, .init = 0}> yaw;
    halp::knob_f32<"Pitch", halp::range{.min = -180.0, .max = 180.0, .init = 0}> pitch;
    halp::knob_f32<"Roll", halp::range{.min = -180.0, .max = 180.0, .init = 0}> roll;
  } inputs;

  struct
  {
    halp::dynamic_audio_bus<"Output", float> audio;
  } outputs;

  using setup = halp::setup;
  void prepare(halp::setup info)
  {
    nSamples = info.frames;

    M_rot_tmp.resize(max_nsh * max_nsh);
    y.resize(max_nsh);
  }

  using tick = halp::tick;

  void operator()(halp::tick t);

  struct ui;

private:
  int order, nSH, nSamples;
  float azi, elev, yaw, pitch, roll;
  std::vector<float> M_rot_tmp{}, y{};
  halp::fft<float> FFT;
};

}
