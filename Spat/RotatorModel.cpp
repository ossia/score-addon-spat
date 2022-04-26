#include "RotatorModel.hpp"

namespace Spat{
void Rotator::operator()(halp::tick t)
{
    if (inputs.audio.channels == 0)
      return;

    order = inputs.order;
    nSH = (order + 1) * (order + 1);
    nSamples = t.frames;

    yaw = inputs.yaw * M_PI / 180.0;
    pitch = inputs.pitch * M_PI / 180.0;
    roll = inputs.roll * M_PI / 180.0;

    /*while(nSH > inputs.audio.channels)
        {
            order -= 1;
            nSH = (order+1)*(order+1);
        }*/

    float** in = inputs.audio.samples;
    float** out = outputs.audio.samples;

    float inFrame[max_nsh][nSamples];
    float M_rot[max_nsh][max_nsh];

    float Rxyz[3][3];
    float M_rot_tmp[max_nsh * max_nsh];

    for (int i = 0; i < nSamples; i++)
        for (int j = 0; j < nSH; j++)
            inFrame[j][i] = in[0][i];

    //for (int j = 0; j < nSH; j++)
    //  std::copy_n(in[0], nSamples, inFrame[j]);

    yawPitchRoll2Rzyx(yaw, pitch, roll, 1, Rxyz);
    getSHrotMtxReal(Rxyz, (float*)M_rot_tmp, order);

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

    if (!sameRot)
    {
      //fade with linear interpolation
      float tmpFrame[max_nsh][nSamples];
      float fadeIn[nSamples], fadeOut[nSamples];

      for (int i = 0; i < nSamples; i++)
      {
        fadeIn[i] = (float)(i + 1) * 1.0f / (float)nSamples;
        fadeOut[i] = 1.0f - fadeIn[i];
      }

      for (int i = 0; i < nSH; i++)
        for (int j = 0; j < nSamples; j++)
        {
          tmpFrame[i][j] = 0.0f;
          for (int k = 0; k < nSH; k++)
            tmpFrame[i][j] += prevM_rot[i][k] * inFrame[k][j];

          out[i][j] = out[i][j] * fadeIn[j] + tmpFrame[i][j] * fadeOut[j];
          prevM_rot[i][j] = M_rot[i][j];
        }
    }

    for (int i = nSH; i < inputs.audio.channels; i++)
      for (int j = 0; j < nSamples; j++)
        out[i][j] = 0.0f;
}
}
