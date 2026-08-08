#include "_Methods.hpp"

int64_t getFactorial(int n)
{
    int64_t result=1;
    for(int i=1 ; i<=n ; i++)
        result *= i;

    return result;
}

void yawPitchRoll2Rzyx
(
    float yaw,
    float pitch,
    float roll,
    float R[3][3]
)
{
    using namespace std;

    float R1[3][3] = {  { cos(yaw),  sin(yaw), 0.f },
                        { sin(yaw),  cos(yaw), 0.f },
                        { 0.f,       0.f,      1.f } };

    float R2[3][3] = {  { cos(pitch), 0.f, -sin(pitch) },
                        { 0.f,        1.f,         0.f },
                        { sin(pitch), 0.f,  cos(pitch) } };

    float R3[3][3] = {  { 1.f,        0.f,       0.f },
                        { 0.f,  cos(roll), sin(roll) },
                        { 0.f, -sin(roll), cos(roll) } };

    float Rtmp[3][3];

    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        Rtmp[i][j] = 0.0f;
        for (int k = 0; k < 3; k++)
          Rtmp[i][j] += R2[i][k] * R1[k][j];
      }

    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        R[i][j] = 0.0f;
        for (int k = 0; k < 3; k++)
          R[i][j] += R3[i][k] * Rtmp[k][j];
      }
}
