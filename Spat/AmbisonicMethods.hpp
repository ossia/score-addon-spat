#pragma once

#include <cmath>
#include <algorithm>
#include <iostream>
#include <span>

static constexpr int max_FuMA_order = 3;
static constexpr int max_FuMA_nsh = ((max_FuMA_order+1)*(max_FuMA_order+1));

void yawPitchRoll2Rzyx
(
    float yaw,
    float pitch,
    float roll,
    int rollPitchYawFLAG,
    float R[3][3]
);

/* Ivanic, J., Ruedenberg, K. (1998). Rotation Matrices for Real Spherical Harmonics. Direct Determination
 * by Recursion Page: Additions and Corrections. Journal of Physical Chemistry A, 102(45), 9099?9100. */
float getP (int M, int i, int l, int a, int b, float R_1[3][3], float* R_lm1);

/* Ivanic, J., Ruedenberg, K. (1998). Rotation Matrices for Real Spherical Harmonics. Direct Determination
 * by Recursion Page: Additions and Corrections. Journal of Physical Chemistry A, 102(45), 9099?9100. */
float getU (int M, int l, int m, int n, float R_1[3][3], float* R_lm1);

/* Ivanic, J., Ruedenberg, K. (1998). Rotation Matrices for Real Spherical Harmonics. Direct Determination
 * by Recursion Page: Additions and Corrections. Journal of Physical Chemistry A, 102(45), 9099?9100. */
float getV (int M, int l, int m, int n, float R_1[3][3], float* R_lm1);

/* Ivanic, J., Ruedenberg, K. (1998). Rotation Matrices for Real Spherical Harmonics. Direct Determination
 * by Recursion Page: Additions and Corrections. Journal of Physical Chemistry A, 102(45), 9099?9100. */
float getW (int M, int l, int m, int n, float R_1[3][3], float* R_lm1);

//std::vector<float> getSHrotMtxReal
void getSHrotMtxReal
(
    float Rxyz[3][3],
    int L,
    std::vector<float>& RotMtx,
    int size
);

void convertACNtoFUMA(float** in, int order, int nSamples);
void convertFUMAtoACN(float** in, int order, int nSamples);
