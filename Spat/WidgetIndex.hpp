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

namespace Spat
{

class WidgetIndex
{
public:
    halp_meta(name, "WidgetIndex")
    halp_meta(category, "Widget")
    halp_meta(c_name, "widget index")
    halp_meta(uuid, "fd65f345-b478-4ebc-a9dd-e9eb728dfddf")

    struct custom_test;

    struct custom_spatatouille;

    struct custom_audio;

    struct custom_spect;

    struct custom_dome;

    struct custom_matrix;
};
}

