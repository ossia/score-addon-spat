#pragma once

#include <cmath>

static constexpr float deg_to_rad = M_PI / 180.f;
static constexpr float rad_to_deg = 180.f / M_PI;

int64_t getFactorial(int n);

void yawPitchRoll2Rzyx
(
    float yaw,
    float pitch,
    float roll,
    float R[3][3]
);
