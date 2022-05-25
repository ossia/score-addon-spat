#pragma once

#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/meta.hpp>
#include <halp/layout.hpp>

namespace Spat
{

class Test
{
public:
  halp_meta(name, "Test")
  halp_meta(category, "Audio")
  halp_meta(c_name, "test")
  halp_meta(uuid, "0fb284f5-8811-472a-abad-905e1fa0e6db")

  // Define inputs and outputs ports.
  // See the docs at https://github.com/celtera/avendish
  struct ins
  {
    halp::dynamic_audio_bus<"Input", float> audio;
  } inputs;

  struct
  {
    halp::dynamic_audio_bus<"Output", float> audio;
  } outputs;

  using setup = halp::setup;
  void prepare(halp::setup info)
  {
    nFrames = info.frames;
  }

  using tick = halp::tick;

  void operator()(halp::tick t)
  {
      float ** out = outputs.audio.samples;

      //fonctionne jusqu'à i<4, crashe à partir de i<5
       for(int i=0 ; i<4 ; i++)
           for(int j=0 ; j<nFrames ; j++)
               out[i][j] = 1.f;
  }


private :
  int nFrames;
};

}
