#include "Spatatouille.hpp"

namespace Spat
{

void Spatatouille::operator()(halp::tick t)
{
  using namespace std;

  auto volume = inputs.volume;

  double nbr_channels = (double)inputs.audio.channels;

  if (nbr_channels <= 1)
    return;

  auto** in = inputs.audio.samples;
  auto** out = outputs.audio.samples;

  auto pos_x = m_local_data.pos_xy.x;
  auto pos_y = m_local_data.pos_xy.y;
  auto pos_z = inputs.z;

  auto* l_in = inputs.audio[0];
  auto* r_in = inputs.audio[1];

  auto* l_out = outputs.audio[0];
  auto* r_out = outputs.audio[1];

  double l_volume = 0;
  double r_volume = 0;

  int i = 0;

  for (int j = 0; j < t.frames; j++)
  {
    l_out[j] = (-4 * pos_y * (pos_y - 1)) * volume * l_in[j] * (1 - pos_x);
    r_out[j] = (-4 * pos_y * (pos_y - 1)) * volume * r_in[j] * (pos_x);

    outputs.output = -(1 - pos_x) + (pos_x);

    l_volume += pow(l_out[j], 2);
    r_volume += pow(r_out[j], 2);


    fprintf(stderr,"Source (Model) : %d \n" ,m_local_data.source);
  }

  if (t.frames == 0)
    return;
  l_volume = sqrt(l_volume / t.frames);
  r_volume = sqrt(r_volume / t.frames);

  send_message(processor_to_ui{
      .l_volume = l_volume, .r_volume = r_volume, .dome_volume = l_volume});
}
}
