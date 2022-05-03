#include "Mosca.hpp"

namespace Spat
{

void Mosca::operator()(halp::tick t)
{
    auto volume = inputs.volume;

    auto pos_x = m_local_data.pos_xy.x;
    auto pos_y = m_local_data.pos_xy.y;
    auto pos_z = inputs.z;

    auto* l_in = inputs.audio[0];
    auto* r_in = inputs.audio[1];

    auto* l_out = outputs.audio[0];
    auto* r_out = outputs.audio[1];

    for (int j = 0; j < t.frames; j++)
    {
        l_out[j] = (-4 * pos_y * (pos_y-1)) * volume * l_in[j] * (1 - pos_x);
        r_out[j] = (-4 * pos_y * (pos_y-1)) * volume * r_in[j] * (pos_x);

        outputs.output = -(1 - pos_x) + (pos_x);
    }
}
}
