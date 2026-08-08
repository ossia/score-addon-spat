#pragma once

#include <Spat/WidgetIndex.hpp>

namespace Spat
{

struct custom_audio
{
  double volume = 0;

  double nbr_channels = 0;

  std::vector<double> channel_volume;

  static constexpr double width() { return 300.; }  // Axe X
  static constexpr double height() { return 300.; } // Axe

  void paint(avnd::painter auto ctx)
  {
    ctx.update();

    /* Background */
    ctx.set_fill_color({120, 120, 120, 255});
    ctx.begin_path();
    ctx.draw_rect(0., 0., width(), height());
    ctx.fill();

    /* Ring color */
    ctx.set_stroke_width(8.);
    int ellipse = 0;
    for (int i = 145; i >= 5; i -= 10)
    {
      if (ellipse == 0)
      {
        ctx.begin_path();
        ctx.set_stroke_color({0, 120, 0, 255});
      }
      else if (ellipse == 3)
      {
        ctx.begin_path();
        ctx.set_stroke_color({100, 220, 0, 255});
      }
      else if (ellipse == 6)
      {
        ctx.begin_path();
        ctx.set_stroke_color({255, 210, 0, 255});
      }
      else if (ellipse == 9)
      {
        ctx.begin_path();
        ctx.set_stroke_color({255, 105, 0, 255});
      }
      else if (ellipse == 12)
      {
        ctx.begin_path();
        ctx.set_stroke_color({255, 0, 0, 255});
      }
      if (!(i % 10 == 0))
      {
        ctx.draw_circle(150, 150, i);
      }
      ctx.stroke();
      ellipse++;
    }

    /* Parameters */
    double x = 0;
    double y = 0;

    double w = width();
    double h = height();

    /* Volume */
    ctx.set_fill_color({85, 85, 85, 255});
    ctx.begin_path();
    for (int i = 1; i <= nbr_channels; i++)
    {
      ctx.move_to(150, 150);

      fprintf(
          stderr, "AUDIO Channel %d : %f \n", i - 1, channel_volume.at(i - 1));

      if (channel_volume.at(i - 1) > 1)
      {
        channel_volume.at(i - 1) = 1;
      }
      ctx.arc_to(
          x + 150 * (channel_volume.at(i - 1)),
          y + 150 * (channel_volume.at(i - 1)),
          w - 2 * 150 * (channel_volume.at(i - 1)),
          h - 2 * 150 * (channel_volume.at(i - 1)),
          ((i - 1) / nbr_channels) * 360,
          360 / nbr_channels);
    }
    ctx.fill();

    /* Borders*/
    ctx.begin_path();
    ctx.set_stroke_color({45, 45, 45, 255});
    ctx.set_stroke_width(2.);
    for (int i = 10; i <= 150; i += 10)
    {
      ctx.draw_circle(150, 150, i);
    }
    ctx.stroke();
    ctx.begin_path();

    for (int i = 1; i <= nbr_channels; i++)
    {
      ctx.move_to(150, 150);
      ctx.arc_to(
          x, y, w, h, ((i - 1) / nbr_channels) * 360, 360 / nbr_channels);
    }
    ctx.stroke();

    ctx.update();
  }
};
}
