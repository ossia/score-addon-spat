#pragma once
#include <Spat/ExampleModel.hpp>
#include <Spat/WidgetMatrix.hpp>
#include <halp/layout.hpp>

namespace Spat
{
struct Example::ui
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
                fprintf(stderr, "Sending message \"Reset Color\" from UI thread !\n");
                this->process_color(ui);
            };

            ui.customColorR.color.on_pressed = [&]
            {
                fprintf(stderr, "Sending message \"More Red\" from UI thread !\n");
                this->process_colorR(ui);
            };

            ui.customColorG.color.on_pressed = [&]
            {
                fprintf(stderr, "Sending message \"More Green\" from UI thread !\n");
                this->process_colorG(ui);
            };

            ui.customColorB.color.on_pressed = [&]
            {
                fprintf(stderr, "Sending message \"More Blue\" from UI thread !\n");
                this->process_colorB(ui);
            };
        }

        static void process_message(ui& self)
        {
            fprintf(stderr, "Got message in UI thread !\n");
            self.click.button.press_count++;
        }

        static void process_color(ui& self)
        {
            self.customColor.color.colorR = 0;
            self.customColor.color.colorG = 0;
            self.customColor.color.colorB = 0;
            self.customColorR.color.color = 0;
            self.customColorG.color.color = 0;
            self.customColorB.color.color = 0;

            self.click.button.press_count = 0;
        }

        static void process_colorR(ui& self)
        {
            self.customColorR.color.color++;
            self.customColor.color.colorR++;
        }

        static void process_colorG(ui& self)
        {
            self.customColorG.color.color++;
            self.customColor.color.colorG++;
        }

        static void process_colorB(ui& self)
        {
            self.customColorB.color.color++;
            self.customColor.color.colorB++;
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
        halp_meta(name, "CustomColor")
        halp_meta(layout, halp::layouts::vbox)
        halp_meta(background, halp::colors::mid)

        halp::custom_actions_item<custom_color> color{.x = 0, .y = 0};
    } customColor;

    struct {
        halp_meta(name, "CustomColorR")
        halp_meta(layout, halp::layouts::vbox)
        halp_meta(background, halp::colors::mid)

        halp::custom_actions_item<custom_colorR> color{.x = 0, .y = 0};
    } customColorR;

    struct {
        halp_meta(name, "CustomColorG")
        halp_meta(layout, halp::layouts::vbox)
        halp_meta(background, halp::colors::mid)

        halp::custom_actions_item<custom_colorG> color{.x = 0, .y = 0};
    } customColorG;

    struct {
        halp_meta(name, "CustomColorB")
        halp_meta(layout, halp::layouts::vbox)
        halp_meta(background, halp::colors::mid)

        halp::custom_actions_item<custom_colorB> color{.x = 0, .y = 0};
    } customColorB;

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

    struct {
        halp_meta(name, "Morpion")
        halp_meta(layout, halp::layouts::vbox)
        halp_meta(background, halp::colors::mid)

        halp::custom_item<WidgetIndex::custom_matrix, &ins::level> widget{{.x = 500, .y = 920}};
    } custom_morpion;

};
}
