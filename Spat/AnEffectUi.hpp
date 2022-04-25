#pragma once
#include <Spat/AnEffectModel.hpp>
#include <halp/layout.hpp>

namespace Spat
{
struct AnEffect::ui
{
    halp_meta(name, "Main")
    halp_meta(layout, halp::layouts::hbox)
    halp_meta(background, "background.svg")
    halp_meta(width, 600)
    halp_meta(height, 300)
    halp_meta(font, "Inconsolata")

    struct {
        halp_meta(name, "Mosca")
        halp_meta(layout, halp::layouts::vbox)
        halp_meta(background, halp::colors::mid)

        halp::custom_item<custom_mosca, &ins::level> widget{{.x = 500, .y = 920}};
    } mosca;

    struct {
        halp_meta(name, "Panel")
        halp_meta(layout, halp::layouts::vbox)
        halp_meta(width, 300)
        halp_meta(height, 300)

        halp::item<&ins::volume> w0;
        halp::item<&ins::level> w1;
        halp::item<&ins::dopler> w2;
        halp::item<&ins::concentration> w3;
        halp::item<&ins::dstAmount> w4;
    } panel;
};
}
