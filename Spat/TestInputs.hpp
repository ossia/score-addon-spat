#pragma once

#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/meta.hpp>
#include <halp/layout.hpp>
#include <iostream>

namespace Spat
{

class TestInputs
{
public:
  halp_meta(name, "Test Inputs")
  halp_meta(category, "Audio")
  halp_meta(c_name, "test_inputs")
  halp_meta(uuid, "0fb284f5-8211-472a-abad-905e1fa0e6db")

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
        //affiche 0 si connecté à rien et sinon toujours 2, peu importe les sorties du plugin précédent
        std::cout<<inputs.audio.channels<<std::endl;
  }

private :
  int nFrames;
};

}
