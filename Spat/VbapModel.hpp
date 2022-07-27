#pragma once

#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/meta.hpp>
#include <halp/fft.hpp>

#include <iostream>
#include <algorithm>
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
        halp::hslider_i32<"Room - 0:Normal room, 1:Listening room, 2:Anechoic room", halp::range{.min = 0, .max = 2, .init = 0}>
            room;
        halp::knob_f32<"Azimuth", halp::range{.min = -180.0, .max = 180.0, .init = 0}>
            azi;
        halp::knob_f32<"Elevation", halp::range{.min = -180.0, .max = 180.0, .init = 0}>
            elev;
        halp::knob_f32<"Yaw", halp::range{.min = -180.0, .max = 180.0, .init = 0}>
            yaw;
        halp::knob_f32<"Pitch", halp::range{.min = -180.0, .max = 180.0, .init = 0}>
            pitch;
        halp::knob_f32<"Roll", halp::range{.min = -180.0, .max = 180.0, .init = 0}>
            roll;
    } inputs;

    struct
    {
        halp::dynamic_audio_bus<"Output", double> audio;
    } outputs;

    struct ui;

    halp::setup setup_info;
    void prepare(halp::setup info)
    {
        //nSources=inputs.audio.channels;
        nChannels=outputs.audio.channels;

        gains2D.resize(nChannels);
        vbap_gtable.resize(nChannels);
        //G_src.resize();
    }

    void operator()(halp::tick tick);
    
private:
    int nChannels, nSources, nSamples;
    int room;
    float azi, elev;
    float yaw, pitch, roll;
    halp::fft<double> FFT;
    std::vector<double> gains2D{}, vbap_gtable{};
    std::vector<std::vector<double>> src_dirs_xyz;
    std::vector<std::vector<std::complex<double>>> G_src{};
};
}

