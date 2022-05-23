#pragma once

#include <Spat/SpatatouilleModel.hpp>

namespace Spat
{

using namespace std;

struct Spatatouille::custom_spect
{
    static constexpr double width() { return 300.; } // Axe X
    static constexpr double height() { return 300.; } // Axe Y

    halp::xy_type<float> pos;
    int num = 0;
    int num_current = 1;

    std::function<void(halp::xy_type<float>)> on_moved = [] (auto) {};
    std::function<void(int)> source = [] (auto) {};
    std::function<void()> on_pressed = [] { };

    void paint(avnd::painter auto ctx)
    {
        double c_x = width()/2;
        double c_y = height()/2;
        double c_r = 150;
        double c_r_bis = 4;

        float m_x = pos.x * 150;// * width();
        float m_y = pos.y * 15;// * height();

        float m_r = 15.;

        ctx.set_stroke_color({200, 200, 200, 255});
        ctx.set_stroke_width(2.);
        ctx.set_fill_color({120, 120, 120, 255});
        ctx.begin_path();
        ctx.draw_rect(0., 0., width(), height());
        ctx.fill();
        ctx.stroke();

        ctx.begin_path();
        ctx.set_fill_color({255, 255, 255, 255});
        ctx.draw_rect(5, 5, width()-10, height()-10);
        ctx.fill();



        ctx.set_stroke_color({170, 170, 170, 255});
        ctx.set_stroke_width(1.);
        ctx.begin_path();

        for(int i = 15; i<= 285; i+=25)
        {
            ctx.draw_line(i, 15, i, 285);
        }

        for(int i = 285; i>= 15; i-=25)
        {
            ctx.draw_line(15, i, 285, i);
        }

        ctx.stroke();

        /* Axes */
        ctx.set_stroke_color({0, 0, 0, 255});
        ctx.begin_path();
        ctx.draw_line(15, 15, 15, 285);
        ctx.draw_line(15, 15, 10, 20);
        ctx.draw_line(15, 15, 20, 20);
        ctx.draw_line(15, 285, 285, 285);
        ctx.draw_line(285, 285, 280, 280);
        ctx.draw_line(285, 285, 280, 290);
        ctx.stroke();

        ctx.begin_path();
        ctx.set_stroke_color({0, 255, 0, 255});
        ctx.set_stroke_width(2.);
        halp::xy_type<float> p0 = {15, 175};
        float test = 0;
        for (int i = 1; i <275; i++)
        {
            halp::xy_type<float> p1 = {p0.x + rand()%3, test + rand()%150 + 50};

            ctx.draw_line(p0.x, p0.y, p1.x, p1.y);
            p0.x = p1.x;
            p0.y = p1.y;
            ctx.stroke();
        }

    }

    bool mouse_press(double x, double y, auto button)
    {
//        mouse_move(x, y, button);
//        on_moved(pos);
        return true;
    }

    void mouse_move(double x, double y, auto button)
    {
//        pos.x = std::clamp(x / width(), 0., 1.);
//        pos.y = std::clamp(y / height(), 0., 1.);

//        on_moved(pos);
    }

    void mouse_release(double x, double y, auto button)
    {
//        mouse_move(x, y, button);
//        on_moved(pos);
    }
};
}
