#pragma once
#include <Spat/SpatatouilleModel.hpp>
#include <Spat/WidgetSpatatouille.hpp>
#include <Spat/WidgetAudioSpat.hpp>

namespace Spat
{
struct Spatatouille::ui
{
  using enum halp::colors;
  using enum halp::layouts;

  halp_meta(name, "Main")
  halp_meta(layout, halp::layouts::grid)
  halp_meta(columns, 3)
  halp_meta(width, 900)
  halp_meta(height, 300)
  halp_meta(font, "Inconsolata")

  struct bus {
      void init(ui& ui)
      {
        ui.spatatouille.widget.on_moved = [&] (halp::xy_type<float> pos)
        {
            this->send_message(ui_to_processor{.pos_xy = pos});
        };

        ui.spatatouille.widget.on_pressed = [&]
        {
            fprintf(stderr, "On pressed \n");
        };
        ui.spatatouille.widget.source = [&] (int src)
        {
            this->send_message(ui_to_processor{.source = src});
        };
      }

      static void process_message(ui& self){}

      std::function<void(ui_to_processor)> send_message;
  };

  struct {
      halp_meta(name, "Spatatouille")
      halp_meta(layout, halp::layouts::vbox)
      halp_meta(background, halp::colors::mid)
      halp_meta(width, 300)
      halp_meta(height, 300)

      halp::custom_actions_item<Spatatouille::custom_spatatouille> widget{.x = 0, .y = 0};
  } spatatouille;

  struct {
      halp_meta(name, "Audio")
      halp_meta(layout, halp::layouts::vbox)
      halp_meta(background, halp::colors::mid)
      halp_meta(width, 300)
      halp_meta(height, 300)

      halp::custom_actions_item<Spatatouille::custom_audio> widget{.x = 0, .y = 0};
  } audio;

  struct {
      halp_meta(name, "Option")
      halp_meta(layout, halp::layouts::vbox)
      halp_meta(width, 300)
      halp_meta(height, 300)

      halp::item<&ins::volume> volume;
      halp::item<&ins::z> z;

      halp::item<&outs::output> output;
      halp::item<&outs::source> source;
  } option;
};
}
