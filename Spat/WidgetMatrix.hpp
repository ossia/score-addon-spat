#pragma once

#include <Spat/WidgetIndex.hpp>

namespace Spat
{

struct custom_matrix
{
    halp::xy_type<float> pos;

    std::function<void(halp::xy_type<float>)> on_moved = [] (auto) {};

    static constexpr double width() { return 300.; } // X
    static constexpr double height() { return 300.; } // Y

    void paint(avnd::painter auto ctx)
    {
        float m_x = pos.x * width();
        float m_y = pos.y * height();
        float m_r = 16.;      

        ctx.update();

        /* Background */
        ctx.set_fill_color({114, 5, 35, 255});
        ctx.begin_path();
        ctx.draw_rect(0., 0., width(), height());
        ctx.fill();

        /* Grid */
        for(int x = 19; x < width(); x += 38)
        {
          for(int y = 19; y < height(); y += 38)
          {
              ctx.begin_path();
              ctx.set_fill_color({202, 127, 127, 255});
              ctx.draw_circle(x, y, m_r);
              ctx.fill();
          }
        }

        /* Point */
        for(int x = 19; x < width(); x += 38)
        {
          for(int y = 19; y < height(); y += 38)
          {
              float formula = sqrt(pow((m_x - x), 2) + pow((m_y - y), 2));
              if(formula < m_r)
              {
                  ctx.begin_path();
                  ctx.set_fill_color({255, 0, 0, 255});
                  ctx.draw_circle(x, y, m_r);
                  ctx.fill();
              }
          }
        }
        ctx.update();
    }

    bool mouse_press(auto event)
    {
        mouse_move(event);
        on_moved(pos);
        return true;
    }

    bool mouse_move(auto event)
    {
        pos.x = std::clamp(event.x / width(), 0., 1.);
        pos.y = std::clamp(event.y / height(), 0., 1.);
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

