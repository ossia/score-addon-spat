#include <Spat/MonoToAmbiModel.hpp>

namespace Spat{

void MonoToAmbi::operator()(halp::tick t)
{
    if (inputs.audio.channels <= 0 || nSamples == 0)
      return;

    FuMA = !inputs.conv;
    order = inputs.order;
    nSH = (order + 1) * (order + 1);

    while(nSH > outputs.audio.channels())
    {
        order -= 1;
        nSH = (order+1)*(order+1);
        std::cout<<"order -= 1 : order = "<<order<<" ; nSH = "<<nSH<<std::endl;
    }

    float** in = inputs.audio.samples;
    float** out = outputs.audio.samples;

    float Rxyz[3][3];

    // convert input signal into RSH
    azi = inputs.azi;
    elev = inputs.elev;
    std::vector<float> y(nSH);
    std::fill(y.begin(), y.end(), 0.0f);

    getRSH(order, azi, elev, y);

    for (int i = 0; i < nSH; i++)
        for (int j = 0; j < nSamples; j++)
            outVec[i][j] = y[i] * in[0][j];

    if(FuMA)
        convertACNtoFUMA(outVec, order, nSamples);

    for (int i = 0; i < nSH; i++)
        std::copy(outVec[i].begin(), outVec[i].end(), out[i]);

    for (int i = nSH; i < outputs.audio.channels(); i++)
      for (int j = 0; j < nSamples; j++)
        out[i][j] = 0.0f;
}
}
