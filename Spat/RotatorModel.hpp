#pragma once

#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/meta.hpp>

#include <iostream>
#include "AmbisonicMethods.hpp"
//#include <saf.h>

namespace Spat{

class Rotator
{
    static constexpr int max_order = 7;
    static constexpr int max_nsh = ((max_order+1)*(max_order+1));

public:
    halp_meta(name, "Rotator")
    halp_meta(category, "Audio")
    halp_meta(c_name, "avnd_rotator")
    halp_meta(uuid, "82bdb9f5-9cf8-440e-8675-c0caf4fc59b9")

    using setup = halp::setup;
    using tick = halp::tick;

    struct ins
    {
        halp::dynamic_audio_bus<"Input", float> audio;
        halp::hslider_i32<"Order", halp::range{.min = 0, .max = max_order, .init = 0}>
            order;
        halp::knob_f32<"Yaw", halp::range{.min = -180.0, .max = 180.0, .init = 0}>
            yaw;
        halp::knob_f32<"Pitch", halp::range{.min = -180.0, .max = 180.0, .init = 0}>
            pitch;
        halp::knob_f32<"Roll", halp::range{.min = -180.0, .max = 180.0, .init = 0}>
            roll;
    } inputs;

    struct
    {
        halp::dynamic_audio_bus<"Output", float> audio;
    } outputs;

    struct ui;

    halp::setup setup_info;
    void prepare(halp::setup info)
    {
        nSamples=info.frames;

        inFrame.resize(max_nsh, std::vector<float>(nSamples));
        tmpFrame.resize(max_nsh, std::vector<float>(nSamples));
        M_rot.resize(max_nsh, std::vector<float>(max_nsh));
        prevM_rot.resize(max_nsh, std::vector<float>(max_nsh));

        fadeIn.resize(nSamples);
        fadeOut.resize(nSamples);

        //M_rot_tmp.resize(max_nsh * max_nsh);
    };

    void operator()(halp::tick tick);

private:
    int order, nSH, nSamples;
    float yaw, pitch, roll;
    std::vector<std::vector<float>> inFrame{}, tmpFrame{}, M_rot{}, prevM_rot{};
    std::vector<float> fadeIn{}, fadeOut{};

    //std::vector<float> M_rot_tmp{};
};
}

