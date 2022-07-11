#pragma once

#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/meta.hpp>
#include <halp/fft.hpp>

#include <iostream>
#include <Spat/_VbapMethods.hpp>

namespace Spat{

class Vbap
{
public:
    halp_meta(name, "VBAP")
    halp_meta(category, "Audio")
    halp_meta(c_name, "avnd_vbap")
    halp_meta(uuid, "82bdb3c5-9cf8-440e-8675-c0caf4fc59b9")

    using setup = halp::setup;
    using tick = halp::tick;

    struct ins
    {
        halp::dynamic_audio_spectrum_bus<"Input", double>
            audio;
        halp::knob_f32<"Azimuth", halp::range{.min = -180.0, .max = 180.0, .init = 0}>
            azi;
        halp::knob_f32<"Elevation", halp::range{.min = -180.0, .max = 180.0, .init = 0}>
            elev;
    } inputs;

    struct
    {
        halp::dynamic_audio_bus<"Output", double> audio;
    } outputs;

    struct ui;

    halp::setup setup_info;
    void prepare(halp::setup info)
    {
        nSources=inputs.audio.channels;
        nChannels=outputs.audio.channels;
    };
    void operator()(halp::tick tick);
    
private:
    int nChannels, nSources, nSamples;
    float azi, elev;
    halp::fft<double> FFT;
};
}

