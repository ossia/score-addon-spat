#pragma once

#include <Spat/WidgetIndex.hpp>

namespace Spat
{

struct custom_spec_display
{
  float zoom = 1;

  static constexpr double width() { return 300.; }
  static constexpr double height() { return 300.; }

  void paint(avnd::painter auto ctx)
  {
    ctx.set_fill_color({0, 202, 169, 255});
    ctx.begin_path();
    for (std::size_t c = 0; c < spectrums.size(); c++)
    {
      auto& spectrum = spectrums[c];
      const double h = height() / spectrums.size();
      const double barw = width() / (spectrum.size() / zoom);

      for (std::size_t i = 0; i < spectrum.size(); i++)
      {
        double barh = std::pow(spectrum[i], 1. / 4.);
        ctx.draw_rect(i * barw, c * h + (h - barh * h), barw, barh * h);
      }
    }
    ctx.fill();
    ctx.update();
  }

  std::vector<std::vector<float>> spectrums;
};
}
