#pragma once

#include <Spat/SpatatouilleModel.hpp>

namespace Spat
{

using namespace std;

struct AnEffect::paint
{
    struct custom_morpion
    {
        static constexpr double width() { return 300.; } // X
        static constexpr double height() { return 300.; } // Y

        void set_value(const auto& control, halp::xy_type<float> value)
        {
          this->value = avnd::map_control_to_01(control, value);
        }

        static auto value_to_control(auto& control, halp::xy_type<float> value)
        {
          return avnd::map_control_from_01(control, value);
        }

        void paint(avnd::painter auto ctx)
        {
            float m_x = value.x * width();
            float m_y = value.y * height();
            float m_r = 16.;

            ctx.set_fill_color({114, 5, 35, 255});
            ctx.begin_path();
            ctx.draw_rect(0., 0., width(), height());
            ctx.fill();

            for(int x = 19; x < width(); x += 38)
            {
              for(int y = 19; y < height(); y += 38)
              {
                  ctx.begin_path();
                  //ctx.set_stroke_color({255, 255, 255, 255});
                  ctx.set_fill_color({202, 127, 127, 255});
                  ctx.draw_circle(x, y, m_r);
                  ctx.fill();
                  //ctx.stroke();
              }
            }

            for(int x = 19; x < width(); x += 38)
            {
              for(int y = 19; y < height(); y += 38)
              {
                  float formula = sqrt(pow((m_x - x), 2) + pow((m_y - y), 2));
                  if(formula < m_r)
                  {
                      ctx.begin_path();
                      //ctx.set_stroke_color({255, 255, 255, 255});
                      ctx.set_fill_color({255, 0, 0, 255});
                      ctx.draw_circle(x, y, m_r);
                      ctx.fill();
                      //ctx.stroke();
                  }
              }
            }
        }

        bool mouse_press(double x, double y)
        {
            transaction.start();
            mouse_move(x, y);
            return true;
        }

        void mouse_move(double x, double y)
        {
            halp::xy_type<float> res;
            res.x = std::clamp(x / width(), 0., 1.);

            res.y = std::clamp(y / height(), 0., 1.);
            transaction.update(res);
        }

        void mouse_release(double x, double y)
        {
            //mouse_move(x, y);
            transaction.commit();
        }

        halp::transaction<halp::xy_type<float>> transaction;
        halp::xy_type<float> value{};
    };

};

}

