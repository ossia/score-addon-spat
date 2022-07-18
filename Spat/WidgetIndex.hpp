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

#include <Spat/WidgetSpatatouille.hpp>
#include <Spat/WidgetAudioSpat.hpp>
#include <Spat/WidgetSpectSpat.hpp>
#include <Spat/WidgetDbBar.hpp>
#include <Spat/WidgetDbBarVu.hpp>
#include <Spat/WidgetDomeSpat.hpp>
#include <Spat/WidgetMatrix.hpp>
#include <Spat/WidgetSpecDisplay.hpp>

namespace Spat
{
    struct custom_spatatouille;

    struct custom_audio;

    struct custom_spect;

    struct custom_dome;

    struct custom_matrix;

    struct custom_db_bar;

    struct custom_db_bar_vu;

    struct custom_spec_display;
}

