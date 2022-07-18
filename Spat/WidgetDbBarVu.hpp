#pragma once

#include <Spat/WidgetIndex.hpp>

namespace Spat
{

/*
 * Work in progress...
 */

struct custom_db_bar_vu
{
    halp::xy_type<float> pos;

    std::function<void(halp::xy_type<float>)> on_moved = [] (auto) {};

    static constexpr double width() { return 300.; } // Axe X
    static constexpr double height() { return 300.; } // Axe Y

    void paint(avnd::painter auto ctx)
    {
        ctx.update();

        /* Background */
        ctx.set_fill_color({120, 120, 120, 255});
        ctx.begin_path();
        ctx.draw_rect(0., 0., width(), height());
        ctx.fill();

        ctx.set_fill_color({255, 255, 185, 255});
        ctx.begin_path();
        ctx.draw_rect(5., 5., width()-10, 200);
        ctx.fill();

        ctx.set_stroke_color({0, 0, 0, 255});
        ctx.set_stroke_width(3.);
        ctx.begin_path();
        ctx.draw_rect(5., 5., width()-10, 200);
        ctx.stroke();

        /* Black */
        ctx.set_stroke_color({0, 0, 0, 255});
        ctx.set_stroke_width(2.);
        ctx.begin_path();
        ctx.move_to(254., 107);
        ctx.arc_to(5., 75., width()-10, 200, 45, 90);
        ctx.draw_line(46, 107, 26, 87);
        ctx.stroke();

        /* Red */
        ctx.set_stroke_color({255, 0, 0, 255});
        ctx.set_stroke_width(2.);
        ctx.begin_path();
        ctx.move_to(254., 107);
        ctx.arc_to(5., 75., width()-10, 200, 45, 30);
        ctx.draw_line(254., 107, 274, 87);
        ctx.stroke();


        ctx.update();
    }
};
}
