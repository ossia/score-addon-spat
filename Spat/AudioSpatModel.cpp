#include "AudioSpat.hpp"

namespace Spat
{

void AudioSpat::operator()(halp::tick t)
{
  using namespace std;

  auto volume = inputs.volume;

  double nbr_channels = (double)inputs.audio.channels;

  if (nbr_channels <= 0)
    return;

  auto** in = inputs.audio.samples;
  auto** out = inputs.audio.samples;

  std::vector<double> channel_volume;

  for (int j = 0; j < t.frames; j++)
  {
    for (int k = 0; k < nbr_channels; k++)
    {
      out[k][j] = in[k][j] * volume;
      channel_volume.push_back(pow(out[k][j], 2));
    }
  }

  if (t.frames == 0)
    return;

  for (int k = 0; k < nbr_channels; k++)
  {
    channel_volume[k] = sqrt(channel_volume[k] / t.frames) * 100;
  }

  send_message(
      processor_to_ui{.channel_volume = channel_volume, .nbr_channels = nbr_channels});
}
}
