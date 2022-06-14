/* Most of these methods are modified versions of Leo McCormack's SAF library : https://github.com/leomccormack/Spatial_Audio_Framework
* Copyright 2020-2021 Leo McCormack
*
* Permission to use, copy, modify, and/or distribute this software for any
* purpose with or without fee is hereby granted, provided that the above
* copyright notice and this permission notice appear in all copies.
*/

#pragma once

#include <cmath>
#include <algorithm>
#include <iostream>
#include <span>

static constexpr int max_FuMA_order = 3;
static constexpr int max_FuMA_nsh = ((max_FuMA_order+1)*(max_FuMA_order+1));

void unnorm_legendreP (int n, float cos, std::vector<float>& y);
void getSHreal (int order, float azimuth, float inclination, std::vector<float>& Y);
void getRSH (int N, float azi, float elev, std::vector<float>& Y);

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
float getU (int M, int l, int m, int n, float R_1[3][3], float* R_lm1);
float getV (int M, int l, int m, int n, float R_1[3][3], float* R_lm1);
float getW (int M, int l, int m, int n, float R_1[3][3], float* R_lm1);

void getSHrotMtxReal
(
    float Rxyz[3][3],
    int L,
    std::vector<float>& RotMtx,
    int size
);

void convertACNtoFUMA(std::vector<std::vector<float>>& in, int order, int nSamples);
void convertFUMAtoACN(std::vector<std::vector<float>>& in, int order, int nSamples);
