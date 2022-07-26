#pragma once

#include <avnd/concepts/painter.hpp>
#include <avnd/concepts/processor.hpp>
#include <avnd/wrappers/controls.hpp>
#include <cmath>
#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/custom_widgets.hpp>
#include <halp/layout.hpp>
#include <halp/meta.hpp>

#include <cstdio>

namespace Spat
{

class AudioSpat
{
public:
  halp_meta(name, "AudioSpat")
  halp_meta(category, "Audio")
  halp_meta(c_name, "audio_spat")
  halp_meta(uuid, "2152994d-4aa1-4502-a531-06e39c62a3be")

  struct ui_to_processor
  {
    halp::xy_type<float> pos_xy;
    int source;
  };

  struct processor_to_ui
  {
    std::vector<double> channel_volume;
    double nbr_channels;
  };

  ui_to_processor m_local_data{.pos_xy = {.x = 0.5, .y = 1.}};

  struct ins
  {
    halp::dynamic_audio_bus<"Input", double> audio;

    halp::hslider_f32<"Volume", halp::range{.min = 0., .max = 3., .init = 1.}> volume;
  } inputs;

  struct outs
  {
    halp::dynamic_audio_bus<"Output", double> audio;
  } outputs;

  void process_message(const ui_to_processor& m) { m_local_data = m; }

  std::function<void(processor_to_ui)> send_message;

  using tick = halp::tick;

  void operator()(halp::tick t);

  struct ui;
};
}
