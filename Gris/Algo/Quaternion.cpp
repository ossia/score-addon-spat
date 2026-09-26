#include <Gris/Algo/Quaternion.hpp>

#include <cmath>

namespace Gris
{
Quaternion
getQuaternionFromEulerAngles(float yawParam, float pitchParam, float rollParam) noexcept
{
  float const yawDeg{pitchParam};
  float const pitchDeg{yawParam};
  float const rollDeg{rollParam};

  float const yaw = yawDeg * radians_t::RADIAN_PER_DEGREE * -0.5f;
  float const pitch = pitchDeg * radians_t::RADIAN_PER_DEGREE * 0.5f;
  float const roll = rollDeg * radians_t::RADIAN_PER_DEGREE * 0.5f;

  float const sinYaw = std::sin(yaw);
  float const cosYaw = std::cos(yaw);
  float const sinPitch = std::sin(pitch);
  float const cosPitch = std::cos(pitch);
  float const sinRoll = std::sin(roll);
  float const cosRoll = std::cos(roll);
  float const cosPitchCosRoll = cosPitch * cosRoll;
  float const sinPitchSinRoll = sinPitch * sinRoll;

  return Quaternion{
      cosYaw * sinPitch * cosRoll - sinYaw * cosPitch * sinRoll,
      sinYaw * cosPitchCosRoll + cosYaw * sinPitchSinRoll,
      cosYaw * cosPitch * sinRoll + sinYaw * sinPitch * cosRoll,
      -(cosYaw * cosPitchCosRoll - sinYaw * sinPitchSinRoll)};
}
}
