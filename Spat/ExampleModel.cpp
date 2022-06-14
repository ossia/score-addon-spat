#include <Spat/Example.hpp>

namespace Spat
{
void Example::operator()(halp::tick t)
{
  // Process the input buffer
  for (int i = 0; i < inputs.audio.channels; i++)
  {
    auto* in = inputs.audio[i];
    auto* out = outputs.audio[i];

    for (int j = 0; j < t.frames; j++)
    {
      out[j] = inputs.volume * in[j];
    }
  }
}
}
