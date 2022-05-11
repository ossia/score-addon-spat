#pragma once

#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/meta.hpp>

#include <iostream>
#include <Spat/AmbisonicMethods.hpp>

namespace Spat{

class MonoToAmbi
{
    static constexpr int max_order = 7;
    static constexpr int max_nsh = ((max_order+1)*(max_order+1));
    static constexpr float deg_to_rad = 2. * M_PI / 360.;

public:
    halp_meta(name, "Mono To Ambisonics")
    halp_meta(category, "Audio")
    halp_meta(c_name, "avnd_monotoambi")
    halp_meta(uuid, "82bdb9f5-9cf8-440e-6675-c0caf4fc59b9")

    using setup = halp::setup;
    using tick = halp::tick;

    struct ins
    {
        halp::dynamic_audio_bus<"Input", float> audio;
        halp::hslider_i32<"Channel order convention (FuMA / ACN)", halp::range{.min = 0, .max = 1, .init = 0}>
            conv;
        halp::knob_f32<"Azimuth", halp::range{.min = -180.0, .max = 180.0, .init = 0}>
            azi;
        halp::knob_f32<"Elevation", halp::range{.min = -180.0, .max = 180.0, .init = 0}>
            elev;
        halp::hslider_i32<"Order", halp::range{.min = 0, .max = max_order, .init = 0}>
            order;
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

        outVec.resize(max_nsh, std::vector<float>(nSamples));
    };

    void operator()(halp::tick tick);

private:
    int FuMA, order, nSH, nSamples;
    float azi, elev;
    std::vector<std::vector<float>> outVec{};
};
}

