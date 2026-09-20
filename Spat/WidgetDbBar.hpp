#pragma once

#include <Spat/WidgetIndex.hpp>

namespace Spat
{

struct custom_db_bar
{
  double l_volume = 0;
  double r_volume = 0;

  static constexpr double width() { return 300.; }  // Axe X
  static constexpr double height() { return 300.; } // Axe Y

  double random_gen()
  {
    double val = (double)rand() / RAND_MAX;

    if (val < 0.20)
      return 50;
    else if (val < 0.40)
      return 120;
    else if (val < 0.75)
      return 170;
    else if (val < 0.9)
      return 220;
    else
      return 260;
  }

  void paint(avnd::painter auto ctx)
  {
    ctx.update();

    double l_x = 115;
    double r_x = 165;

    /* Background */
    ctx.set_fill_color({120, 120, 120, 255});
    ctx.begin_path();
    ctx.draw_rect(0., 0., width(), height());
    ctx.fill();

    /*** Left part ***/
    /* Background dB */
    ctx.begin_path();
    ctx.set_linear_gradient(
        l_x, 20., l_x, 150., {255, 0, 0, 255}, {200, 175, 0, 255});
    ctx.draw_rect(l_x, 20., 20., 130.);
    ctx.fill();

    ctx.begin_path();
    ctx.set_linear_gradient(
        l_x, 150., l_x, 280., {200, 175, 0, 255}, {0, 255, 0, 255});
    ctx.draw_rect(l_x, 150., 20., 130.);
    ctx.fill();

    /* Mask */
    ctx.set_fill_color({85, 85, 85, 255});
    ctx.begin_path();
    if (l_volume > 1)
    {
      l_volume = 1;
    }
    ctx.draw_rect(l_x, 20., 20., 260 * (1 - l_volume));
    ctx.fill();

    /*** Right part ***/
    /* Background dB */
    ctx.begin_path();
    ctx.set_linear_gradient(
        r_x, 20., r_x, 150., {255, 0, 0, 255}, {200, 175, 0, 255});
    ctx.draw_rect(r_x, 20., 20., 130.);
    ctx.fill();
    ctx.begin_path();
    ctx.set_linear_gradient(
        r_x, 150., r_x, 280., {200, 175, 0, 255}, {0, 255, 0, 255});
    ctx.draw_rect(r_x, 150., 20., 130.);
    ctx.fill();

    /* Mask */
    ctx.set_fill_color({85, 85, 85, 255});
    ctx.begin_path();
    if (r_volume > 1)
    {
      r_volume = 1;
    }
    ctx.draw_rect(r_x, 20., 20., 260 * (1 - r_volume));
    ctx.fill();

    ctx.update();

    /* Text */
    ctx.set_font("Ubuntu");
    ctx.set_font_size(20);
    ctx.set_fill_color({255, 255, 255, 255});
    ctx.begin_path();
    ctx.draw_text(45, 280, "Left");
    ctx.draw_text(205, 280, "Right");
    ctx.fill();

    ctx.update();
  }
};
}
