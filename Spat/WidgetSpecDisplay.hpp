#pragma once

#include <Spat/WidgetIndex.hpp>

namespace Spat
{

struct custom_spect_example
{
  static constexpr double width() { return 500.; }
  static constexpr double height() { return 100.; }

  void paint(avnd::painter auto ctx)
  {
    ctx.set_fill_color({200, 200, 200, 255});
    ctx.begin_path();
    for(std::size_t c = 0; c < spectrums.size(); c++)
    {
      auto& spectrum = spectrums[c];
      double h = height() / spectrums.size();

      for(std::size_t i = 0; i < spectrum.size(); i++)
      {
        double barw = width() / spectrum.size();
        ctx.draw_rect(i * barw, c * h + (h - spectrum[i] * h), barw, spectrum[i] * h);
      }
    }
    ctx.fill();
  }

  std::vector<std::vector<float>> spectrums;
};
}
