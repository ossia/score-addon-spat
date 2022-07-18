#pragma once

#include <Spat/SpecDisplayModel.hpp>
#include <Spat/WidgetIndex.hpp>

namespace Spat
{
struct SpecDisplay::ui
{
  using enum halp::colors;
  using enum halp::layouts;

  halp_meta(name, "Main")
  halp_meta(layout, halp::layouts::grid)
  halp_meta(columns, 3)
  halp_meta(width, 600)
  halp_meta(height, 300)
  halp_meta(font, "Inconsolata")

  struct bus {
    static void process_message(ui& self, processor_to_ui msg)
    {
      self.spectral.widget.spectrums = msg.spectrums;
      self.spectral.widget.zoom = msg.zoom;
    }

  };

  struct {
      halp_meta(name, "Spectral")
      halp_meta(layout, halp::layouts::vbox)
      halp_meta(background, halp::colors::mid)
      halp_meta(width, 300)
      halp_meta(height, 300)

      halp::custom_actions_item<custom_spec_display> widget{.x = 0, .y = 0};
  } spectral;

  struct {
      halp_meta(name, "Option")
      halp_meta(layout, halp::layouts::vbox)
      halp_meta(width, 300)
      halp_meta(height, 300)

      halp::item<&ins::zoom> zoom;
  } option;
};
}
