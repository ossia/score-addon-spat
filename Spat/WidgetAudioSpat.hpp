#pragma once

#include <Spat/WidgetIndex.hpp>

namespace Spat
{

struct custom_audio
{
    double volume;

    static constexpr double width() { return 300.; } // Axe X
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

            }
            ctx.stroke();
            ellipse++;
        }

        /* Parameters */
        double x = 0;
        double y = 0;

        double w = width();
        double h = height();

        double channel = 1;
        double max_channel = 16;

        double rand_gen;

        /* Volume */
        ctx.set_fill_color({85, 85, 85, 255});
        ctx.begin_path();
        for(int i = 1; i <= max_channel; i++){
            ctx.move_to(150, 150);
            ctx.arc_to(x + 150 * (volume) , y + 150 * (volume), w - 2 * 150 * (volume), h - 2 * 150 * (volume), ((i-1) / max_channel)*360, 360 / max_channel);
        }
        ctx.fill();

        /* Borders*/
        ctx.begin_path();
        ctx.set_stroke_color({45, 45, 45, 255});
        ctx.set_stroke_width(2.);
        for(int i = 10; i<= 150; i+=10){
            ctx.draw_circle(150, 150, i);
        }
        ctx.stroke();
        ctx.begin_path();
        for(int i = 1; i <= max_channel; i++){
            ctx.move_to(150, 150);
            ctx.arc_to(x, y, w, h, ((i-1) / max_channel)*360, 360 / max_channel);
        }
        ctx.stroke();

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
