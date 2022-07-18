#pragma once

#include <avnd/concepts/ui.hpp>
#include <avnd/concepts/processor.hpp>
#include <avnd/concepts/painter.hpp>
#include <avnd/wrappers/controls.hpp>
#include <halp/custom_widgets.hpp>
#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/layout.hpp>
#include <halp/meta.hpp>
#include <cmath>
#include <variant>
#include <cstdio>

namespace Spat
{

class SpecDisplay
{
public:
  halp_meta(name, "SpecDisplay")
  halp_meta(category, "Audio")
  halp_meta(c_name, "spec_display")
  halp_meta(uuid, "27418bc9-5e40-4d10-8daf-0f3493f07271")

  struct ins
  {
    halp::dynamic_audio_spectrum_bus<"In", double> audio;
    static_assert(avnd::spectrum_split_bus_port<decltype(audio)>);

    halp::hslider_f32<"Zoom", halp::range{.min = 1., .max = 20., .init = 1.}> zoom;
  } inputs;

  struct outs
  {
  } outputs;

  struct processor_to_ui
  {
    std::vector<std::vector<float>> spectrums;
    float zoom;
  };

  std::function<void(processor_to_ui)> send_message;

  void operator()(int N);

  struct ui;
};
}
