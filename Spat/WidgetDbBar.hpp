#pragma once

#include <Spat/WidgetIndex.hpp>

namespace Spat
{

struct custom_db_bar
{
    static constexpr double width() { return 300.; } // Axe X
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

        /* Background */
        ctx.set_fill_color({120, 120, 120, 255});
        ctx.begin_path();
        ctx.draw_rect(0., 0., width(), height());
        ctx.fill();

        /* Outline */
        ctx.set_fill_color({255, 255, 255, 255});
        ctx.begin_path();
        ctx.draw_rect(30., 15., 60., 270.);
        ctx.fill();

        /* Background dB */
        ctx.begin_path();
        ctx.set_linear_gradient(35., 20., 35., 150., {255, 0, 0, 255}, {200, 175, 0, 255});
        ctx.draw_rect(35., 20., 50., 130.);
        ctx.fill();
        ctx.begin_path();
        ctx.set_linear_gradient(35., 150., 35., 280., {200, 175, 0, 255}, {0, 255, 0, 255});
        ctx.draw_rect(35., 150., 50., 130.);
        ctx.fill();

        /* Mask */
        ctx.set_fill_color({255, 255, 255, 255});
        ctx.begin_path();
        ctx.draw_rect(35., 20., 50., random_gen());
        ctx.fill();

        /* Text */
        ctx.begin_path();
//        ctx.set_font('Ubuntu');
        ctx.set_font_size(20.);
//        ctx.draw_text(150., 150., (std::string_view)random_gen());
        ctx.draw_text(170., 150., "20 dB");
        ctx.fill();

        ctx.update();
    }

    bool mouse_press(double x, double y, auto button)
    {
        return true;
    }

    void mouse_move(double x, double y, auto button)
    {
    }

    void mouse_release(double x, double y, auto button)
    {
    }
};
}
