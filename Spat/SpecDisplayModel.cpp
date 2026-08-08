#include "SpecDisplay.hpp"

namespace Spat
{

void SpecDisplay::operator()(int N)
{
    const auto channels = inputs.audio.channels;
    processor_to_ui p;
    p.spectrums.resize(channels);
    p.zoom = inputs.zoom;

    for(int i = 0; i < channels; i++)
    {
      auto& chan = p.spectrums[i];
      chan.resize(N/2);
      auto& ampl = inputs.audio.spectrum.amplitude[i];
      auto& ph = inputs.audio.spectrum.phase[i];
      for(int k = 0; k < N / 2; k++) {
        chan[k] = std::clamp(ampl[k] * ampl[k] + ph[k] * ph[k], 0., 1.);
      }
    }


    send_message(std::move(p));
}
}
