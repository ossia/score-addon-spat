#pragma once
#include <Spat/AnEffectModel.hpp>
#include <halp/layout.hpp>

namespace Spat
{
struct AnEffect::ui
{
    halp_meta(name, "Main")
    halp_meta(layout, halp::layouts::grid)
    halp_meta(columns, 3)
    halp_meta(background, "background.svg")
    halp_meta(width, 900)
    halp_meta(height, 900)
    halp_meta(font, "Inconsolata")

    struct bus {
        void init(ui& ui)
        {
            ui.click.button.on_pressed = [&]
            {
                fprintf(stderr, "Sending message from UI thread !\n");
                this->send_message(ui_to_processor{.utp = 1});
                this->process_message(ui);
            };

            ui.customColor.color.on_pressed = [&]
            {
                fprintf(stderr, "Sending message (customColor) from UI thread !\n");
                this->process_color(ui);
            };
        }

        static void process_message(ui& self)
        {
            fprintf(stderr, "Got message in UI thread !\n");
            self.click.button.press_count++;
        }

        static void process_color(ui& self)
        {
            self.customColor.color.color++;
        }

        std::function<void(ui_to_processor)> send_message;

    };

    struct {
        halp_meta(name, "Click")
        halp_meta(layout, halp::layouts::vbox)
        halp_meta(width, 300)
        halp_meta(height, 300)

        halp::custom_actions_item<custom_button> button{.x = 10, .y = 10};
    } click;

    struct {
        halp_meta(name, "Mosca")
        halp_meta(layout, halp::layouts::vbox)
        halp_meta(background, halp::colors::mid)

        halp::custom_item<custom_mosca, &ins::level> widget{{.x = 500, .y = 920}};
    } mosca;

    struct {
        halp_meta(name, "CustomColor")
        halp_meta(layout, halp::layouts::vbox)
        halp_meta(background, halp::colors::mid)

        halp::custom_actions_item<custom_color> color{.x = 0, .y = 0};
    } customColor;

    struct {
        halp_meta(name, "Panel")
        halp_meta(layout, halp::layouts::vbox)
        halp_meta(width, 300)
        halp_meta(height, 300)

        halp::item<&ins::x> w11;

        halp::item<&ins::volume> w0;
        halp::item<&ins::level> w1;
        halp::item<&ins::dopler> w2;
        halp::item<&ins::concentration> w3;
        halp::item<&ins::dstAmount> w4;
    } panel;
};
}
