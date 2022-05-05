#pragma once

#include <Spat/SpatatouilleModel.hpp>

namespace Spat
{

using namespace std;

struct Spatatouille::circle_ui
{
    struct custom_spatatouille
    {
        static constexpr double width() { return 300.; } // Axe X
        static constexpr double height() { return 300.; } // Axe Y

        halp::xy_type<float> pos;

        std::function<void(halp::xy_type<float>)> on_moved = [] (auto) {};

        void paint(avnd::painter auto ctx)
        {
            double c_x = width()/2;
            double c_y = height()/2;
            double c_r = 150;
            double c_r_bis = 4;

            float m_x = pos.x * width();
            float m_y = pos.y * height();
            float m_r = 15.;

            ctx.set_stroke_color({200, 200, 200, 255});
            ctx.set_stroke_width(2.);
            ctx.set_fill_color({120, 120, 120, 255});
            ctx.begin_path();
            ctx.draw_rect(0., 0., width(), height());
            ctx.fill();
            ctx.stroke();

            ctx.begin_path();
            ctx.set_stroke_color({255, 255, 255, 255});
            ctx.set_fill_color({255, 255, 255, 255});
            ctx.draw_circle(c_x, c_y, c_r);
            ctx.fill();
            ctx.stroke();

            ctx.begin_path();
            ctx.set_stroke_color({255, 255, 255, 255});
            ctx.set_fill_color({0, 0, 0, 255});
            ctx.draw_circle(c_x, c_y, c_r_bis);
            ctx.fill();
            ctx.stroke();

            float formula = sqrt(pow((m_x - c_x), 2) + pow((m_y - c_y), 2));
            if(formula + (m_r - 5) < c_r){
                ctx.begin_path();
                ctx.set_fill_color({90, 90, 90, 255});
                ctx.draw_circle(m_x, m_y, m_r);
                ctx.fill();
                ctx.close_path();

                ctx.begin_path();
                ctx.set_fill_color({255, 255, 255, 255});
                ctx.set_font("Ubuntu");
                ctx.set_font_size(15);
                ctx.draw_text(m_x-6, m_y+7, "1");
                ctx.fill();
            }
        }

        bool mouse_press(double x, double y)
        {
            mouse_move(x, y);
            on_moved(pos);
            return true;
        }

        void mouse_move(double x, double y)
        {
            pos.x = std::clamp(x / width(), 0., 1.);
            pos.y = std::clamp(y / height(), 0., 1.);
            on_moved(pos);
        }

        void mouse_release(double x, double y)
        {
            mouse_move(x, y);
            on_moved(pos);
        }

    };
};
}
