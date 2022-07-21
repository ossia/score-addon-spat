#pragma once

#include <Spat/AudioSpatModel.hpp>
#include <Spat/WidgetIndex.hpp>

namespace Spat
{
struct AudioSpat::ui
{
  using enum halp::colors;
  using enum halp::layouts;

  halp_meta(name, "Main") halp_meta(layout, halp::layouts::grid)
      halp_meta(columns, 2) halp_meta(width, 600) halp_meta(height, 300)
          halp_meta(font, "Inconsolata")

              struct bus
  {
    void init(ui& ui) { }

    static void process_message(ui& ui, processor_to_ui op)
    {
      ui.audio.widget.channel_volume = op.channel_volume;
      ui.audio.widget.nbr_channels = op.nbr_channels;
    }

    std::function<void(ui_to_processor)> send_message;
  };

  struct
  {
    halp_meta(name, "Audio") halp_meta(layout, halp::layouts::vbox)
        halp_meta(background, halp::colors::mid) halp_meta(width, 300)
            halp_meta(height, 300)

                halp::custom_actions_item<custom_audio> widget{.x = 0, .y = 0};
  } audio;

  struct
  {
    halp_meta(name, "Option") halp_meta(layout, halp::layouts::vbox)
        halp_meta(width, 300) halp_meta(height, 300)

            halp::item<&ins::volume> volume;
    halp::item<&outs::audio> output;
  } option;
};
}
