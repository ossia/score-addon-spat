#include "Mosca.hpp"

namespace Spat
{

void Mosca::operator()(halp::tick t)
{
  // Process the input buffer
  for (int i = 0; i < inputs.audio.channels; i++)
  {
    auto* in = inputs.audio[i];
    auto* out = outputs.audio[i];

    for (int j = 0; j < t.frames; j++)
    {
      float test = mosca.result_res_x();

      out[j] = test * in[j];

      fprintf(stderr, "MoscaModel.cpp, x = %f \n", m_local_data.valx);
    }
  }
}
}
