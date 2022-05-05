#include "RotatorModel.hpp"

namespace Spat{



void Rotator::operator()(halp::tick t)
{
    if (inputs.audio.channels <= 0 || nSamples == 0)
      return;

    FuMA = !inputs.conv;
    order = inputs.order;
    nSH = (order + 1) * (order + 1);

    yaw = inputs.yaw * deg_to_rad;
    pitch = inputs.pitch * deg_to_rad;
    roll = inputs.roll * deg_to_rad;

    while(nSH > outputs.audio.channels)
    {
        order -= 1;
        nSH = (order+1)*(order+1);
    }

    float** in = inputs.audio.samples;
    float** out = outputs.audio.samples;

    float Rxyz[3][3];

    for (int i = 0; i < nSamples; i++)
        for (int j = 0; j < nSH; j++)
            inFrame[j][i] = in[0][i];

    yawPitchRoll2Rzyx(yaw, pitch, roll, 0, Rxyz);
    getSHrotMtxReal(Rxyz, order, M_rot_tmp, max_nsh*max_nsh);

    bool sameRot = true;

    for (int i = 0; i < nSH; i++)
      for (int j = 0; j < nSH; j++)
      {
        M_rot[i][j] = M_rot_tmp[i * nSH + j];
        if (M_rot[i][j] != prevM_rot[i][j])
          sameRot = false;
      }

    for (int i = 0; i < nSH; i++)
      for (int j = 0; j < nSamples; j++)
      {
        out[i][j] = 0.0f;
        for (int k = 0; k < nSH; k++)
          out[i][j] += M_rot[i][k] * inFrame[k][j];
      }

    const float inv_nSamples = 1.f / nSamples;

    if (!sameRot)
    {
      //fade with linear interpolation
      for (int i = 0; i < nSamples; i++)
      {
        fadeIn[i] = (float)(i + 1) * inv_nSamples;
        fadeOut[i] = 1.0f - fadeIn[i];
      }

      for (int i = 0; i < nSH; i++)
      {
        for (int j = 0; j < nSamples; j++)
        {
          tmpFrame[i][j] = 0.0f;
          for (int k = 0; k < nSH; k++)
            tmpFrame[i][j] += prevM_rot[i][k] * inFrame[k][j];
          out[i][j] = out[i][j] * fadeIn[j] + tmpFrame[i][j] * fadeOut[j];
        }

        for (int j=0 ; j<nSH ; j++)
            prevM_rot[i][j] = M_rot[i][j];
      }
    }

    if(FuMA)
        convertACNtoFUMA(out, order, nSamples);

    for (int i = nSH; i < inputs.audio.channels; i++)
      for (int j = 0; j < nSamples; j++)
        out[i][j] = 0.0f;
}
}
