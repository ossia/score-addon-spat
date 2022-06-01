#pragma once

#include <Spat/WidgetIndex.hpp>

namespace Spat
{

using namespace std;

struct WidgetIndex::custom_spatatouille
{
    static constexpr double width() { return 300.; } // Axe X
    static constexpr double height() { return 300.; } // Axe Y

    halp::xy_type<float> pos, pos_1, pos_2, pos_3;
    int num = 0;
    int num_current = 1;

    std::function<void(halp::xy_type<float>)> on_moved = [] (auto) {};
    std::function<void(int)> source = [] (auto) {};

    void paint(avnd::painter auto ctx)
    {
        double c_x = width()/2;
        double c_y = height()/2;
        double c_r = 150;
        double c_r_bis = 4;

        float m_x_1 = pos_1.x * width();
        float m_y_1 = pos_1.y * height();

        float m_x_2 = pos_2.x * width();
        float m_y_2 = pos_2.y * height();

        float m_x_3 = pos_3.x * width();
        float m_y_3 = pos_3.y * height();

        float m_r = 15.;  

        ctx.update();

        /* Background */
        ctx.set_fill_color({120, 120, 120, 255});
        ctx.begin_path();
        ctx.draw_rect(0., 0., width(), height());
        ctx.fill();

        /* Circle background */
        ctx.begin_path();
        ctx.set_fill_color({255, 255, 255, 255});
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

        /* 1st source */
        float formula_1 = sqrt(pow((m_x_1 - c_x), 2) + pow((m_y_1 - c_y), 2));
        if(formula_1 < c_r){
            ctx.begin_path();
            ctx.set_fill_color({90, 90, 90, 255});
            ctx.draw_circle(m_x_1, m_y_1, m_r);
            ctx.fill();
            ctx.close_path();

            ctx.begin_path();
            ctx.set_fill_color({255, 255, 255, 255});
            ctx.set_font("Ubuntu");
            ctx.set_font_size(15);
            ctx.draw_text(m_x_1-6, m_y_1+7, "1");
            ctx.fill();
        }
        ctx.update();

        /* 2nd source */
        float formula_2 = sqrt(pow((m_x_2 - c_x), 2) + pow((m_y_2 - c_y), 2));
        if(formula_2 < c_r){
            ctx.begin_path();
            ctx.set_fill_color({90, 90, 90, 255});
            ctx.draw_circle(m_x_2, m_y_2, m_r);
            ctx.fill();
            ctx.close_path();

            ctx.begin_path();
            ctx.set_fill_color({255, 255, 255, 255});
            ctx.set_font("Ubuntu");
            ctx.set_font_size(15);
            ctx.draw_text(m_x_2-6, m_y_2+7, "2");
            ctx.fill();
        }
        ctx.update();

        /* 3rd source */
        float formula_3 = sqrt(pow((m_x_3 - c_x), 2) + pow((m_y_3 - c_y), 2));
        if(formula_3 < c_r){
            ctx.begin_path();
            ctx.set_fill_color({90, 90, 90, 255});
            ctx.draw_circle(m_x_3, m_y_3, m_r);
            ctx.fill();
            ctx.close_path();

            ctx.begin_path();
            ctx.set_fill_color({255, 255, 255, 255});
            ctx.set_font("Ubuntu");
            ctx.set_font_size(15);
            ctx.draw_text(m_x_3-6, m_y_3+7, "3");
            ctx.fill();
        }
        ctx.update();
    }

    bool mouse_press(double x, double y, auto button)
    {

        on_moved(pos);

        if (button == 1){
            mouse_move(x, y, button);
        }else if (button == 2){

            fprintf(stderr, "Num current = %i \n", num_current);
            if(num == 0){
                if(num_current == 3){
                    num_current = 1;
                    source(num_current);
                }else if(num_current == 1){
                    num_current = 2;
                    source(num_current);
                }else if(num_current == 2){
                    num_current = 3;
                    source(num_current);
                }
                num = 1;
            }else{
                num = 0;
            }
        }

        return true;
    }

    void mouse_move(double x, double y, auto button)
    {
        pos.x = std::clamp(x / width(), 0., 1.);
        pos.y = std::clamp(y / height(), 0., 1.);

        if (num_current == 1){
            pos_1.x = pos.x;
            pos_1.y = pos.y;
        }else if (num_current == 2){
            pos_2.x = pos.x;
            pos_2.y = pos.y;
        }else if(num_current == 3){
            pos_3.x = pos.x;
            pos_3.y = pos.y;
        }

        on_moved(pos);
    }

    void mouse_release(double x, double y, auto button)
    {
        mouse_move(x, y, button);
        on_moved(pos);
    }
};
}
