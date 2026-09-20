#include <Spat/VbapModel.hpp>

namespace Spat
{
void Vbap::operator()(halp::tick t)
{
  if (inputs.audio.channels <= 0 || nSamples == 0
      || outputs.audio.channels < 2)
    return;

  double** in = inputs.audio.samples;
  double** out = outputs.audio.samples;

  // convert input signal into RSH
  azi = inputs.azi;
  elev = inputs.elev;

  std::complex<double>** specs = (std::complex<double>**)alloca(
      sizeof(std::complex<double>) * (nChannels * nSamples));
  double* specOut = (double*)alloca(sizeof(double) * nSamples);

  for (int i = 0; i < nChannels; i++)
  {
    for (int j = 0; j < nSamples; j++)
      specOut[j] = in[0][j];
    specs[i] = FFT.execute(specOut, nSamples);
  }

  //algo ici
  /*

        }*/

  for (int i = 0; i < nChannels; i++)
  {
    out[i] = FFT.execute(specs[i], nSamples);
    for (int j = 0; j < nSamples; j++)
      out[i][j] /= nSamples; // normalisation
  }
}
}
