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

class Example
{
public:
  halp_meta(name, "Test-examples")
  halp_meta(category, "Audio")
  halp_meta(c_name, "test_examples")
  halp_meta(uuid, "0fb2b4f5-8811-472a-abad-905e1fa0e6db")

  struct custom_button
  {
      static constexpr double width() { return 300.; }
      static constexpr double height() { return 300.; }

      void paint(avnd::painter auto ctx)
      {
        ctx.set_stroke_color({200, 200, 200, 255});
        ctx.set_stroke_width(2.);
        ctx.set_fill_color({100, 100, 100, 255});
        ctx.begin_path();
        ctx.draw_rounded_rect(0., 0., width(), height(), 5);
        ctx.fill();
        ctx.stroke();

        ctx.set_fill_color({0, 0, 0, 255});
        ctx.begin_path();
        ctx.set_font("Ubuntu");
        ctx.set_font_size(press_count +5);
        ctx.draw_text(150.- press_count*0.95, 150. - press_count*0.95, fmt::format("{}", press_count));
        ctx.fill();

        ctx.update();
      }

      bool mouse_press(double x, double y)
      {
        on_pressed();
        return true;
      }

      void mouse_move(double x, double y)
      {
          on_pressed();
      }

      int press_count{0};
      std::function<void()> on_pressed = [] { };
  };

  struct custom_color
  {
      static constexpr double width() { return 300.; }
      static constexpr double height() { return 300.; }

      void paint(avnd::painter auto ctx)
      {
        uint8_t colR = static_cast<uint8_t>(colorR);
        uint8_t colG = static_cast<uint8_t>(colorG);
        uint8_t colB = static_cast<uint8_t>(colorB);
        ctx.set_stroke_color({200, 200, 200, 255});
        ctx.set_stroke_width(2.);
        ctx.set_fill_color({colR, colG, colB, 255});
        ctx.begin_path();
        ctx.draw_rounded_rect(0., 0., width(), height(), 5);
        ctx.fill();
        ctx.stroke();

        ctx.update();
      }

      bool mouse_press(double x, double y)
      {
        on_pressed();
        return true;
      }

      void mouse_move(double x, double y)
      {
        on_pressed();
      }

      void mouse_release(double x, double y)
      {
      }

      int colorR{0};
      int colorG{0};
      int colorB{0};

      std::function<void()> on_pressed = [] { };
  };

  struct custom_colorR
  {
      static constexpr double width() { return 300.; }
      static constexpr double height() { return 300.; }

      void paint(avnd::painter auto ctx)
      {
        ctx.set_stroke_color({200, 200, 200, 255});
        ctx.set_stroke_width(2.);
        ctx.set_fill_color({255, 0, 0, 255});
        ctx.begin_path();
        ctx.draw_rounded_rect(0., 0., width(), height(), 5);
        ctx.fill();
        ctx.stroke();

        ctx.set_fill_color({0, 0, 0, 255});
        ctx.begin_path();
        ctx.set_font("Ubuntu");
        ctx.set_font_size(55);
        ctx.draw_text(90., 175., fmt::format("{}", color % 255));
        ctx.fill();

        ctx.update();
      }

      bool mouse_press(double x, double y)
      {
        on_pressed();
        return true;
      }

      void mouse_move(double x, double y)
      {
        on_pressed();
      }

      int color{0};
      std::function<void()> on_pressed = [] { };
  };

  struct custom_colorG
  {
      static constexpr double width() { return 300.; }
      static constexpr double height() { return 300.; }

      void paint(avnd::painter auto ctx)
      {
        ctx.set_stroke_color({200, 200, 200, 255});
        ctx.set_stroke_width(2.);
        ctx.set_fill_color({0, 255, 0, 255});
        ctx.begin_path();
        ctx.draw_rounded_rect(0., 0., width(), height(), 5);
        ctx.fill();
        ctx.stroke();

        ctx.set_fill_color({0, 0, 0, 255});
        ctx.begin_path();
        ctx.set_font("Ubuntu");
        ctx.set_font_size(55);
        ctx.draw_text(90., 175., fmt::format("{}", color % 255));
        ctx.fill();

        ctx.update();
      }

      bool mouse_press(double x, double y)
      {
        on_pressed();
        return true;
      }

      void mouse_move(double x, double y)
      {
        on_pressed();
      }

      int color{0};
      std::function<void()> on_pressed = [] { };
  };

  struct custom_colorB
  {
      static constexpr double width() { return 300.; }
      static constexpr double height() { return 300.; }

      void paint(avnd::painter auto ctx)
      {
        ctx.set_stroke_color({200, 200, 200, 255});
        ctx.set_stroke_width(2.);
        ctx.set_fill_color({0, 0, 255, 255});
        ctx.begin_path();
        ctx.draw_rounded_rect(0., 0., width(), height(), 5);
        ctx.fill();
        ctx.stroke();

        ctx.set_fill_color({0, 0, 0, 255});
        ctx.begin_path();
        ctx.set_font("Ubuntu");
        ctx.set_font_size(55);
        ctx.draw_text(90., 175., fmt::format("{}", color % 255));
        ctx.fill();

        ctx.update();
      }

      bool mouse_press(double x, double y)
      {
        on_pressed();
        return true;
      }

      void mouse_move(double x, double y)
      {
        on_pressed();
      }

      int color{0};
      std::function<void()> on_pressed = [] { };
  };


  // Define inputs and outputs ports.
  // See the docs at https://github.com/celtera/avendish
  struct ins
  {
      halp::dynamic_audio_bus<"Input", double> audio;

      halp::hslider_i32<"X", halp::range{.min = 0, .max = 255, .init = 0}> x;
      halp::hslider_f32<"Y", halp::range{.min = -10, .max = 10, .init = 0}> y;

      halp::knob_f32<"Volume", halp::range{.min = 0., .max = 5., .init = 1.}> volume;
      halp::hslider_f32<"Level", halp::range{.min = -10, .max = 10, .init = 0}> level;
      halp::hslider_f32<"Doppler amount", halp::range{.min = -10, .max = 10, .init = 0}> dopler;
      halp::hslider_f32<"Concentration", halp::range{.min = -10, .max = 10, .init = 0}> concentration;
      halp::hslider_f32<"Dst. amount", halp::range{.min = -10, .max = 10, .init = 0}> dstAmount;
  } inputs;

  struct
  {
    halp::dynamic_audio_bus<"Output", double> audio;
  } outputs;

  struct ui_to_processor
  {
      int utp;
      float futp;
  };

  struct processor_to_ui
  {
      int ptu;
      float fptu;
  };

  void process_message()
  {
      fprintf(stderr, "Got message in processing thread !\n");
      send_message(processor_to_ui{.ptu = 1});
  }

  void process_color()
  {
      send_message(processor_to_ui{.ptu = inputs.x});
  }

  std::function<void(processor_to_ui)> send_message;

  using setup = halp::setup;
  void prepare(halp::setup info)
  {
    previous_values.resize(info.input_channels);
  }

  // Do our processing for N samples
  using tick = halp::tick;

  // Defined in the .cpp
  void operator()(halp::tick t);

  // UI is defined in another file to keep things clear.
  struct ui;

  struct paint;

  struct test;

  private:
  // Here we have some state which depends on the host configuration (number of channels, etc).
  std::vector<float> previous_values{};
};
}
