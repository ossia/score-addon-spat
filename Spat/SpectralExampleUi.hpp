#pragma once

#include <Spat/SpectralExampleModel.hpp>
#include <Spat/WidgetIndex.hpp>

namespace Spat
{
struct SpectralExample::ui
{
  using enum halp::colors;
  using enum halp::layouts;

  halp_meta(name, "Main")
  halp_meta(layout, halp::layouts::grid)
  halp_meta(width, 200)
  halp_meta(height, 100)
  halp_meta(font, "Inconsolata")

  struct bus {
      void init(ui& ui, processor_to_ui msg)
      {
        ui.spectral.widget.spectrums = msg.spectrums;
      }

      std::function<void(processor_to_ui)> send_message;
  };

  struct {
      halp_meta(name, "Spectral")
      halp_meta(layout, halp::layouts::vbox)
      halp_meta(background, halp::colors::mid)
      halp_meta(width, 200)
      halp_meta(height, 100)

      halp::custom_actions_item<custom_spect_example> widget{.x = 0, .y = 0};
  } spectral;
};
}
