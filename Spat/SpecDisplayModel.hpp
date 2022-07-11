#pragma once

#include <avnd/concepts/processor.hpp>
#include <avnd/concepts/painter.hpp>
#include <avnd/wrappers/controls.hpp>
#include <halp/custom_widgets.hpp>
#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/layout.hpp>
#include <halp/meta.hpp>
#include <cmath>
#include <cstdio>

namespace Spat
{

class SpecDisplay
{
public:
  halp_meta(name, "SpecDisplay")
  halp_meta(category, "Audio")
  halp_meta(c_name, "spec_display")
  halp_meta(uuid, "5855871d-dfdc-48ee-9225-ca26371ddea4")


  struct {
    halp::dynamic_audio_spectrum_bus<"In", double> audio;
    static_assert(avnd::spectrum_split_bus_port<decltype(audio)>);
  } inputs;

  struct { } outputs;

  struct processor_to_ui {
      std::vector<std::vector<float>> spectrums;
  };

  std::function<void(processor_to_ui)> send_message;

  void operator()(int N);

  struct ui;
};
}
