#include <Spat/StereoPanningModel.hpp>

namespace Spat
{
void StereoPanning::operator()(halp::tick t)
{
  auto coeff = inputs.lr;

  auto* l_in = inputs.audio[0];
  auto* r_in = inputs.audio[1];

  auto* l_out = outputs.audio[0];
  auto* r_out = outputs.audio[1];

  for (int j = 0; j < t.frames; j++)
  {
    l_out[j] = l_in[j] * (1 - coeff);
    r_out[j] = r_in[j] * (1 + coeff);
  }
}
}
