#include "Mosca.hpp"

namespace Spat
{

void Mosca::operator()(halp::tick t)
{
    auto volume = inputs.volume;

    auto* l_in = inputs.audio[0];
    auto* r_in = inputs.audio[1];

    auto* l_out = outputs.audio[0];
    auto* r_out = outputs.audio[1];

    for (int j = 0; j < t.frames; j++)
    {
        l_out[j] = volume * l_in[j] * (1 - m_local_data.pos_xy.x);
        r_out[j] = volume * r_in[j] * (m_local_data.pos_xy.x);
    }
}
}
