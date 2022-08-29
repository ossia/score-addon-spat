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

class Spatatouille
{
public:
  halp_meta(name, "Spatatouille")
  halp_meta(category, "Audio")
  halp_meta(c_name, "spatatouille")
  halp_meta(uuid, "96ab8e9d-9b83-428c-b3fa-7f82b00261dd")

  struct ui_to_processor
  {
//    std::optional<halp::xy_type<float>> pos_xy;
    halp::xy_type<float> pos_xy;
    std::optional<int> source;
  };

  struct processor_to_ui
  {
    double l_volume;
    double r_volume;
    double dome_volume;
  };

  ui_to_processor m_local_data{};

  struct ins
  {
    halp::dynamic_audio_bus<"Input", double> audio;

    halp::hslider_f32<"Volume", halp::range{.min = 0., .max = 3., .init = 1.}> volume;
    halp::vslider_f32<"Z", halp::range{.min = 0., .max = 1., .init = 0.}> z;
  } inputs;

  struct outs
  {
    halp::dynamic_audio_bus<"Output", double> audio;

    halp::hbargraph_f32<
        "Left/Right balance output",
        halp::range{.min = -1., .max = 1., .init = 0.}>
        output;
    halp::hbargraph_i32<"Source", halp::range{.min = 1, .max = 3, .init = 1}> source;
  } outputs;

//  void process_message(const ui_to_processor& m) { m_local_data = m; }

  void process_message(const ui_to_processor& m)
  {
//      if(m.pos_xy)
//          m_local_data.pos_xy = *m.pos_xy;
      if(m.source)
          m_local_data.source = *m.source;
  }

  std::function<void(processor_to_ui)> send_message;

  using tick = halp::tick;

  void operator()(halp::tick t);

  struct ui;
};
}
