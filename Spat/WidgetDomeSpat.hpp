#pragma once

#include <Spat/WidgetIndex.hpp>
#include <cmath>

namespace Spat
{

struct custom_dome
{
    static constexpr double width() { return 300.; } // Axe X
    static constexpr double height() { return 300.; } // Axe Y

    halp::xy_type<float> pos, pos_1, pos_2;
    int num = 0;
    int num_current = 1;

    std::function<void(halp::xy_type<float>)> on_moved = [] (auto) {};
    std::function<void(int)> source = [] (auto) {};

    void paint(avnd::painter auto ctx)
    {
        using namespace std;

        double c_x = width()/2;
        double c_y = height()/2;
        double c_r = 150;
        double c_r_bis = 4;

        ctx.update();

        /* Background */
        ctx.set_fill_color({120, 120, 120, 255});
        ctx.begin_path();
        ctx.draw_rect(0., 0., width(), height());
        ctx.fill();

        /* Circle background */
        ctx.begin_path();
        ctx.set_fill_color({235, 235, 235, 255});
        ctx.draw_circle(c_x, c_y, c_r);
        ctx.fill();

        /* Ring */
        ctx.begin_path();
        ctx.set_stroke_color({150, 150, 150, 10});
        ctx.set_stroke_width(2.5);
        ctx.draw_circle(c_x, c_y, c_r/2);
        ctx.stroke();

        /* Center */
        ctx.begin_path();
        ctx.set_fill_color({0, 0, 0, 255});
        ctx.draw_circle(c_x, c_y, c_r_bis);
        ctx.fill();

        /* Curve */
        ctx.begin_path();
        ctx.set_stroke_color({0, 202, 169, 255});

        double max = 1 * M_PI;
        double theta = atan(pos.y/pos.x)*2 * M_PI;

        halp::xy_type<double> p0 = {cos(theta)+150, sin(theta)+150};
        halp::xy_type<double> p1 = {150, 150};

        for(double i = theta; i< max+theta; i+=0.01){
            double amp = rand()%50+100;
            halp::xy_type<double> p1;
            p1.x = amp * cos(i)+150;
            p1.y = amp * sin(i)+150;

            ctx.draw_line (p0.x, p0.y, p1.x, p1.y);

            p0.x = p1.x;
            p0.y = p1.y;
            ctx.stroke();
        }
        ctx.update();

    }

    bool mouse_press(auto event)
    {
        on_moved(pos);
        if (event.button == event.left){
            mouse_move(event);
        }else if (event.button == event.right){
            if(num_current == 2){
                num_current = 1;
                source(num_current);
            }else if(num_current == 1){
                num_current = 2;
                source(num_current);
            }
        }
        return true;
    }

    bool mouse_move(auto event)
    {
        pos.x = std::clamp(event.x / width(), 0., 1.);
        pos.y = std::clamp(event.y / height(), 0., 1.);
        if (num_current == 1){
            pos_1.x = pos.x;
            pos_1.y = pos.y;
        }else if (num_current == 2){
            pos_2.x = pos.x;
            pos_2.y = pos.y;
        }
        on_moved(pos);
        return true;
    }

    bool mouse_release(auto event)
    {
        mouse_move(event);
        on_moved(pos);
        return true;
    }
};
}
