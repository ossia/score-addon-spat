#pragma once

#include <avnd/concepts/processor.hpp>
#include <avnd/concepts/painter.hpp>
#include <avnd/wrappers/controls.hpp>
#include <halp/custom_widgets.hpp>
#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/layout.hpp>
#include <halp/meta.hpp>
#include <cmath>

#include <cstdio>

//Modif
#include <iostream>
#include <string.h>
#include <math.h>
#include <stdio.h>

namespace Spat
{

class Mosca
{
public:
  halp_meta(name, "Mosca-like")
  halp_meta(category, "Audio")
  halp_meta(c_name, "mosca_like")
  halp_meta(uuid, "96ab8e9d-9b83-428c-b3fa-7f82b00261dd")

  struct custom_mosca
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
          double c_x = width()/2;
          double c_y = height()/2;
          double c_r = 150;
          double c_r_bis = 4;

          float m_x = value.x * width();
          float m_y = value.y * height();
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
          mouse_move(x, y);
          transaction.commit();
      }

      halp::transaction<halp::xy_type<float>> transaction;
      halp::xy_type<float> value{};
  };

  struct ins
  {
    halp::dynamic_audio_bus<"Input", double> audio;

    halp::hslider_f32<"Level", halp::range{.min = -10, .max = 10, .init = 0}> level;
  } inputs;

  struct outs
  {
    halp::dynamic_audio_bus<"Output", double> audio;
  } outputs;

  using setup = halp::setup;
  void prepare(halp::setup info)
  {
    // Initialization, this method will be called with buffer size, etc.
  }

  using tick = halp::tick;

  void operator()(halp::tick t);

  struct ui;
};

}
