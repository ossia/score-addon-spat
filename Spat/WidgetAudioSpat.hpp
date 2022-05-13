#pragma once

#include <Spat/SpatatouilleModel.hpp>

namespace Spat
{

using namespace std;

struct Spatatouille::custom_audio
{
    static constexpr double width() { return 300.; } // Axe X
    static constexpr double height() { return 300.; } // Axe Y

    halp::xy_type<float> pos, pos_1, pos_2, pos_3;
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

        float m_x = pos_1.x * width();
        float m_y = pos_1.y * height();

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
        ctx.set_stroke_width(2.);
        for(int i = 10; i<= 150; i+=10){

            ctx.draw_circle(150, 150, i);

        }
        ctx.stroke();



        ctx.set_stroke_width(8.);
        int ellipse = 0;
        for(int i = 5; i<= 145; i+=10){
            if (ellipse == 0){
                ctx.begin_path();
                ctx.set_stroke_color({255, 0, 0, 70});
            }
            else if (ellipse == 3){
                ctx.begin_path();
                ctx.set_stroke_color({255, 105, 0, 255});
            }
            else if (ellipse == 6){
                ctx.begin_path();
                ctx.set_stroke_color({255, 210, 0, 255});
            }
            else if (ellipse == 9){
                ctx.begin_path();
                ctx.set_stroke_color({100, 220, 0, 255});
            }
            else if (ellipse == 12){
                ctx.begin_path();
                ctx.set_stroke_color({0, 120, 0, 255});
            }
            if(!(i % 10 == 0) && ellipse >= 0){ //2nd condition only for testing
                ctx.draw_circle(150, 150, i);

                ctx.stroke();

            }
            ellipse++;

        }

        ctx.set_stroke_color({255, 255, 255, 255});
        ctx.set_stroke_width(2.);
        ctx.begin_path();
        ctx.draw_line(0, 150, 300, 150);
        ctx.draw_line(150, 0, 150, 300);
        ctx.draw_line(44, 256, 256, 44);
        ctx.draw_line(44, 44, 256, 256);

        ctx.stroke();

        float formula_1 = sqrt(pow((m_x - c_x), 2) + pow((m_y - c_y), 2));

    }

    bool mouse_press(double x, double y, auto button)
    {

//        on_moved(pos);

//        if (button == 1){
//            mouse_move(x, y, button);
//        }else if (button == 2){

//            fprintf(stderr, "Num current = %i \n", num_current);
//            if(num == 0){
//                if(num_current == 3){
//                    num_current = 1;
//                    source(num_current);
//                }else if(num_current == 1){
//                    num_current = 2;
//                    source(num_current);
//                }else if(num_current == 2){
//                    num_current = 3;
//                    source(num_current);
//                }
//                num = 1;
//            }else{
//                num = 0;
//            }
//        }

        return true;
    }

    void mouse_move(double x, double y, auto button)
    {
//        pos.x = std::clamp(x / width(), 0., 1.);
//        pos.y = std::clamp(y / height(), 0., 1.);

//        if (num_current == 1){
//            pos_1.x = pos.x;
//            pos_1.y = pos.y;
//        }else if (num_current == 2){
//            pos_2.x = pos.x;
//            pos_2.y = pos.y;
//        }else if(num_current == 3){
//            pos_3.x = pos.x;
//            pos_3.y = pos.y;
//        }

//        on_moved(pos);
    }

    void mouse_release(double x, double y, auto button)
    {
//        mouse_move(x, y, button);
//        on_moved(pos);
    }
};
}
