#pragma once

#include <Spat/SpatatouilleModel.hpp>

namespace Spat
{

using namespace std;

struct Spatatouille::custom_audio
{
    static constexpr double width() { return 300.; } // Axe X
    static constexpr double height() { return 300.; } // Axe Y

    halp::xy_type<float> pos;
    int num = 0;
    int num_current = 1;

    std::function<void(halp::xy_type<float>)> on_moved = [] (auto) {};
    std::function<void(int)> source = [] (auto) {};
    std::function<void()> on_pressed = [] { };

    double random_gen()
    {
        double val = (double)rand() / RAND_MAX;

        if (val < 0.05)
            return 55;
        else if (val < 0.40)
            return 90;
        else if (val < 0.75)
            return 100;
        else if (val < 0.9)
            return 120;
        else
            return 130;
    }

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

        /* Background color */
        ctx.set_stroke_width(8.);
        int ellipse = 0;
        for(int i = 145; i>= 5; i-=10){
            if (ellipse == 0){
                ctx.begin_path();
                ctx.set_stroke_color({0, 120, 0, 255});
            }
            else if (ellipse == 3){
                ctx.begin_path();
                ctx.set_stroke_color({100, 220, 0, 255});
            }
            else if (ellipse == 6){
                ctx.begin_path();
                ctx.set_stroke_color({255, 210, 0, 255});
            }
            else if (ellipse == 9){
                ctx.begin_path();
                ctx.set_stroke_color({255, 105, 0, 255});
            }
            else if (ellipse == 12){
                ctx.begin_path();
                ctx.set_stroke_color({255, 0, 0, 70});
            }
            if(!(i % 10 == 0)){
                ctx.draw_circle(150, 150, i);

                ctx.stroke();

            }
            ellipse++;

        }

        /* Volume mask */
        ctx.set_stroke_color({200, 200, 200, 255});
        ctx.set_stroke_width(1.);

        for(int i = 0; i<8; i++){
            int x = 151;
            int y = 151;
            int coord_arc = 149;
            int size = 2;
            int random = random_gen();
            for(int j = 0; j<random; j++){
                ctx.begin_path();
                ctx.move_to(x, y);
                ctx.arc_to(coord_arc, coord_arc, size, size, i*45, 45);
                ctx.stroke();
                if ((i == 0) || (i == 7)){
                    x++;
                }else if((i == 1) || (i == 2)){
                    y--;
                }else if((i == 3) || (i == 4)){
                    x--;
                }else if((i == 5) || (i == 6)){
                    y++;
                }
                coord_arc--;
                size+=2;
            }
        }

        /* Edges */
        ctx.set_stroke_color({255, 255, 255, 255});
        ctx.set_stroke_width(2.);
        ctx.begin_path();
        ctx.draw_line(0, 150, 300, 150);
        ctx.draw_line(150, 0, 150, 300);
        ctx.draw_line(44, 256, 256, 44);
        ctx.draw_line(44, 44, 256, 256);
        ctx.stroke();

        ctx.begin_path();
        ctx.set_stroke_color({255, 255, 255, 255});
        ctx.set_stroke_width(2.);
        for(int i = 10; i<= 150; i+=10){

            ctx.draw_circle(150, 150, i);

        }
        ctx.stroke();
    }

    bool mouse_press(double x, double y, auto button)
    {
        mouse_move(x, y, button);
        on_moved(pos);
        return true;
    }

    void mouse_move(double x, double y, auto button)
    {
        pos.x = std::clamp(x / width(), 0., 1.);
        pos.y = std::clamp(y / height(), 0., 1.);

        on_moved(pos);
    }

    void mouse_release(double x, double y, auto button)
    {
        mouse_move(x, y, button);
        on_moved(pos);
    }
};
}
