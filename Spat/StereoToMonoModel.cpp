#include "StereoToMonoModel.hpp"

namespace Spat{
void StereoToMono::operator()(halp::tick t)
{
    // Process the input buffer
    auto* l_in = inputs.audio[0];
    auto* r_in = inputs.audio[1];

    auto* l_out = outputs.audio[0];
    auto* r_out = outputs.audio[1];

    for (int j = 0; j < t.frames; j++)
    {
      auto out = (l_in[j] + r_in[j]) / 2.0f;
      l_out[j] = out;
      r_out[j] = out;
    }
}
}
