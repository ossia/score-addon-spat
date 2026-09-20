#include <Spat/RotatorModel.hpp>

namespace Spat
{

void Rotator::operator()(halp::tick t)
{
  if (inputs.audio.channels <= 0 || nSamples == 0
      || outputs.audio.channels <= 0)
    return;

  FuMA = inputs.conv;
  order = inputs.order;
  nSH = (order + 1) * (order + 1);

  yaw = inputs.yaw * deg_to_rad;
  pitch = inputs.pitch * deg_to_rad;
  roll = inputs.roll * deg_to_rad;

  while (nSH > outputs.audio.channels)
  {
    order -= 1;
    nSH = (order + 1) * (order + 1);
  }

  float** in = inputs.audio.samples;
  float** out = outputs.audio.samples;

  if (order == 0)
  {
    for (int j = 0; j < nSamples; j++)
      out[0][j] = in[0][j];

    for (int i = 1; i < outputs.audio.channels; i++)
      for (int j = 0; j < nSamples; j++)
        out[i][j] = 0.0f;

    return;
  }

  float Rxyz[3][3];

  for (int i = 0; i < nSH; i++)
    for (int j = 0; j < nSamples; j++)
      inFrame[i][j] = in[i][j];

  if (FuMA)
    convertFUMAtoACN(inFrame, order, nSamples);

  yawPitchRoll2Rzyx(yaw, pitch, roll, 0, Rxyz);
  getSHrotMtxReal(Rxyz, order, M_rot_tmp, max_nsh * max_nsh);

  bool sameRot = true;

  //calculate rotation matrix (rotator process)
  for (int i = 0; i < nSH; i++)
    for (int j = 0; j < nSH; j++)
    {
      M_rot[i][j] = M_rot_tmp[i * nSH + j];
      if (M_rot[i][j] != prevM_rot[i][j])
        sameRot = false;
    }

  //apply rotation (rotator process)
  for (int i = 0; i < nSH; i++)
    for (int j = 0; j < nSamples; j++)
    {
      outVec[i][j] = 0.0f;
      for (int k = 0; k < nSH; k++)
        outVec[i][j] += M_rot[i][k] * inFrame[k][j];
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
        outVec[i][j] = outVec[i][j] * fadeIn[j] + tmpFrame[i][j] * fadeOut[j];
      }

      for (int j = 0; j < nSH; j++)
        prevM_rot[i][j] = M_rot[i][j];
    }
  }

  if (FuMA)
    convertACNtoFUMA(outVec, order, nSamples);

  for (int i = 0; i < nSH; i++)
    std::copy(outVec[i].begin(), outVec[i].end(), out[i]);

  for (int i = nSH; i < outputs.audio.channels; i++)
    for (int j = 0; j < nSamples; j++)
      out[i][j] = 0.0f;
}
}
