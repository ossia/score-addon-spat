/* Most of these methods are modified versions of Leo McCormack's SAF library : https://github.com/leomccormack/Spatial_Audio_Framework
* Copyright 2020-2021 Leo McCormack
*
* Permission to use, copy, modify, and/or distribute this software for any
* purpose with or without fee is hereby granted, provided that the above
* copyright notice and this permission notice appear in all copies.
*/

#include <Spat/AmbisonicMethods.hpp>

void unnorm_legendreP
(
    int n,
    float cos,
    std::vector<float>& y
)
{
    using namespace std;

    float* P = (float*) alloca(sizeof(float)*(n+3));
    float* sqrt_n = (float*) alloca(sizeof(float)*(2*n+1));

    if(n==0)
    {
        y[0] = 1.0f;
        return;
    }

    // init
    float s = sqrt(1.0-pow(cos,2.0)) + 2.23e-20;
    float s_n = pow(-s, n);
    float tc = -2.0 * cos/s;

    for(int i=0; i<2*n+1; i++)
        sqrt_n[i] = sqrt(i);
    float norm = 1.0;
    for(int i=1; i<=n; i++)
        norm *= 1.0 - 1.0/(2.0*i);

    // Starting values for downwards recursion
    P[n] = sqrt(norm)*s_n;
    P[n-1] = P[n] * tc * n/sqrt_n[2*n];

    // 3-step downwards recursion to m == 0
    for(int m=n-2; m>=0; m--)
        P[m] = (P[m+1]*tc*(m+1.0) - P[m+2] * sqrt_n[n+m+2] * sqrt_n[n-m-1])/(sqrt_n[n+m+1]*sqrt_n[n-m]);

    // keep up to the last 3 elements in P
    for(int i=0; i<n+1; i++)
        y[i] = P[i];

    // Account for polarity when x == -/+1 for first value of P
    if(sqrt(1.0-pow(cos,2.0))==0)
        y[0] = pow(cos,n);

    // scale each row by: sqrt((n+m)!/(n-m)!)
    for(int m=1; m<n; m++){
        float scale = 1.0;
        for(int i=n-m+1; i<n+m+1; i++)
            scale*=sqrt_n[i];
        y[m] *= scale;
    }

    float scale = 1.0;
    for(int i=1; i<2*n+1; i++)
        scale*=sqrt_n[i];
    y[n] *= scale;
}

int64_t factorial(int n)
{
    int64_t result=1;
    for(int i=1 ; i<=n ; i++)
        result *= i;

    return result;
}

void getSHreal
(
    int order,
    float azimuth,
    float inclination,
    std::vector<float>& Y
)
{
    using namespace std;

    float* Lnm = (float*) alloca(sizeof(float)*(2*order+1));
    float* norm_real = (float*) alloca(sizeof(float)*(2*order+1));
    vector<float> p_nm(order+1);

    float cos_incl = cos(inclination);

    int idx_Y = 0;
    for(int n=0; n<=order; n++){
        // vector of unnormalised associated Legendre functions of current order
        unnorm_legendreP(n, cos_incl, p_nm); // includes Condon-Shortley phase term

        //cancel the Condon-Shortley phase from the definition of the Legendre functions to result in signless real SH
        if (n != 0)
            for(int m=-n, j=0; m<=n; m++, j++)
                Lnm[j] = pow(-1.0, abs(m)) * p_nm[abs(m)];
        else
            Lnm[0] = p_nm[0];

        // normalisation
        for(int m=-n, j=0; m<=n; m++, j++)
            norm_real[j] = sqrt( (2.0*n+1.0) * factorial(n-abs(m)) / (4.0*M_PI*factorial(n+abs(m))) );

        // norm_real * Lnm_real .* CosSin;
        for(int m=-n, j=0; m<=n; m++, j++){
            if(j<n)
                Y[j+idx_Y] = (norm_real[j] * Lnm[j] * sqrt(2.0)*sin((n-j)*azimuth));
            else if(j==n)
                Y[j+idx_Y] = (norm_real[j] * Lnm[j]);
            else /* (j>n) */
                Y[j+idx_Y] = (norm_real[j] * Lnm[j] * sqrt(2.0)*cos((abs(m))*azimuth));
        }

        // increment
        idx_Y = idx_Y + (2*n+1);
    }
}

void getRSH
(
    int N,
    float azi,
    float elev,
    std::vector<float>& Y
)
{
    using namespace std;

    int nSH = (N+1)*(N+1);
    float scale = sqrt(4.0f*M_PI);

    // convert [azi, elev] in degrees, to [azi, inclination] in radians
    const float azimuth = azi*deg_to_rad;
    const float inclination = M_PI/2.0f - elev*deg_to_rad;

    // get real-valued spherical harmonics
    getSHreal(N, azimuth, inclination, Y);

    // remove sqrt(4*pi) term
   for(int i=0 ; i<nSH ; i++)
       Y[i] *= scale;
}

void yawPitchRoll2Rzyx
(
    float yaw,
    float pitch,
    float roll,
    int rollPitchYawFLAG,
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

/* Ivanic, J., Ruedenberg, K. (1998). Rotation Matrices for Real Spherical Harmonics. Direct Determination
 * by Recursion Page: Additions and Corrections. Journal of Physical Chemistry A, 102(45), 9099?9100. */
float getP (int M, int i, int l, int a, int b, float R_1[3][3], float* R_lm1)
{
    float ret = 0.f;

    float ri1 = R_1[i+1][2];
    float rim1 = R_1[i+1][0];
    float ri0 = R_1[i+1][1];

    if (b == -l)
        ret = ri1 * R_lm1[(a+l-1)*M+0] + rim1 * R_lm1[(a+l-1)*M+(2*l-2)];
    else {
        if (b == l)
            ret = ri1*R_lm1[(a+l-1)*M+(2*l-2)] - rim1 * R_lm1[(a+l-1)*M];
        else
            ret = ri0 * R_lm1[(a+l-1)*M+(b+l-1)];
    }

    return ret;
}

/* Ivanic, J., Ruedenberg, K. (1998). Rotation Matrices for Real Spherical Harmonics. Direct Determination
 * by Recursion Page: Additions and Corrections. Journal of Physical Chemistry A, 102(45), 9099?9100. */
float getU(int M, int l, int m, int n, float R_1[3][3], float* R_lm1)
{
    return getP(M, 0, l, m, n, R_1, R_lm1);
}

/* Ivanic, J., Ruedenberg, K. (1998). Rotation Matrices for Real Spherical Harmonics. Direct Determination
 * by Recursion Page: Additions and Corrections. Journal of Physical Chemistry A, 102(45), 9099?9100. */
float getV(int M, int l, int m, int n, float R_1[3][3], float* R_lm1)
{
    using namespace std;

    int d = 0;
    float ret = 0.f, p0 = 0.f, p1 = 0.f;

    if (m == 0) {
        p0 = getP(M, 1, l, 1, n, R_1, R_lm1);
        p1 = getP(M, -1, l, -1, n, R_1, R_lm1);
        ret = p0 + p1;
    }
    else {
        if (m>0) {
            d = m == 1 ? 1 : 0;
            p0 = getP(M, 1, l, m - 1, n, R_1, R_lm1);
            p1 = getP(M, -1, l, -m + 1, n, R_1, R_lm1);
            ret = p0*sqrt(1.0f + d) - p1*(1.0f - d);
        }
        else {
            d = m == -1 ? 1 : 0;
            p0 = getP(M, 1, l, m + 1, n, R_1, R_lm1);
            p1 = getP(M, -1, l, -m - 1, n, R_1, R_lm1);
            ret = p0*(1.0f - (float)d) + p1*sqrt(1.0f + (float)d);
        }
    }

    return ret;
}

/* Ivanic, J., Ruedenberg, K. (1998). Rotation Matrices for Real Spherical Harmonics. Direct Determination
 * by Recursion Page: Additions and Corrections. Journal of Physical Chemistry A, 102(45), 9099?9100. */
float getW (int M, int l, int m, int n, float R_1[3][3], float* R_lm1)
{
    float ret = 0.f, p0 = 0.f, p1 = 0.f;

    if (m != 0) {
        if (m>0) {
            p0 = getP(M, 1, l, m + 1, n, R_1, R_lm1);
            p1 = getP(M, -1, l, -m - 1, n, R_1, R_lm1);
            ret = p0 + p1;
        }
        else {
            p0 = getP(M, 1, l, m - 1, n, R_1, R_lm1);
            p1 = getP(M, -1, l, -m + 1, n, R_1, R_lm1);
            ret = p0 - p1;
        }
    }
    return ret;
}

void getSHrotMtxReal
(
    float Rxyz[3][3],
    int L,
    std::vector<float>& RotMtx,
    int size
)
{
    using namespace std;

    int M =(L+1)*(L+1);
    float* R_lm1 = (float*) alloca(sizeof(float) * M * M);
    float* R_l = (float*) alloca(sizeof(float) * M * M);

    //vector<float> RotMtx(size);
    std::ranges::fill(RotMtx, 0);

    // zeroth-band (l=0) is invariant to rotation
    RotMtx[0] = 1;

    // the first band (l=1) is directly related to the rotation matrix
    float R_1[3][3] = {  { Rxyz[1][1],  Rxyz[1][2], Rxyz[1][0] },
                         { Rxyz[2][1],  Rxyz[2][2], Rxyz[2][0] },
                         { Rxyz[0][1],  Rxyz[0][2], Rxyz[0][0] } };

    for (int i=1; i<4; i++){
        R_lm1[(i-1)*M+0] = R_1[i-1][0];
        R_lm1[(i-1)*M+1] = R_1[i-1][1];
        R_lm1[(i-1)*M+2] = R_1[i-1][2];
        for (int j=1; j<4; j++)
            RotMtx[i*M+j] = R_1[i-1][j-1];
    }

    // compute rotation matrix of each subsequent band recursively
    int bandIdx = 4;
    std::fill(R_l, R_l+M*M, 0);

    for(int l = 2; l<=L; l++){
        for(int m=-l; m<=l; m++){
            for(int n=-l; n<=l; n++){
                // compute u,v,w terms of Eq.8.1 (Table I)
                int d = m == 0 ? 1 : 0; // the delta function d_m0
                int denom = abs(n) == l ? (2*l)*(2*l-1) : (l*l-n*n);

                float u = sqrt( (float)((l*l-m*m)) /  (float)denom);
                float v = sqrt( (float)((1+d)*(l+abs(m)-1)*(l+abs(m))) /  (float)denom) * (float)(1-2*d)*0.5f;
                float w = sqrt( (float)((l-abs(m)-1)*(l-abs(m))) / (float)denom) * (float)(1-d)*(-0.5f);

                // computes Eq.8.1
                if (u!=0)
                    u = u* getU(M,l,m,n,R_1,R_lm1);
                if (v!=0)
                    v = v* getV(M,l,m,n,R_1,R_lm1);
                if (w!=0)
                    w = w* getW(M,l,m,n,R_1,R_lm1);

                R_l[(m+l)*M+(n+l)] = u+v+w;
            }
        }

        for(int i=0; i<2*l+1; i++)
            for(int j=0; j<2*l+1; j++)
                RotMtx[(bandIdx + i)*M + (bandIdx + j)] = R_l[i*M+j];
        for(int i=0; i<2*l+1; i++)
            std::copy_n(R_lm1, M*M, R_l);

        bandIdx += 2*l+1;
    }
}


void convertACNtoFUMA(std::vector<std::vector<float>>& in, int order, int nSamples)
{
    using namespace std;

    if(order <= 0)
        return ;

    int nSH = (order+1)*(order+1);

    vector<vector<float>> ACNsig(nSH, vector<float>(nSamples));
    vector<vector<float>> FuMAsig(nSH, vector<float>(nSamples));

    for(int i=0 ; i<nSH ; i++)
        for(int j=0 ; j<nSamples ; j++)
        {
            ACNsig[i][j] = in[i][j];
            FuMAsig[i][j] = in[i][j];
            //in[i][j] = 0.0f;
        }

    FuMAsig[1].swap(FuMAsig[3]);
    FuMAsig[1].swap(FuMAsig[2]);

    int doneChannels = 4;
    int prevNbChannels = 3;

    while(doneChannels < min(nSH, max_FuMA_nsh))
    {
        int nbChannels = prevNbChannels+2;
        int middleIdx = doneChannels + (nbChannels-1)/2;

        std::copy(ACNsig[doneChannels].begin(), ACNsig[doneChannels].end(), std::back_inserter(FuMAsig[middleIdx]));
        ++doneChannels;

        for(int i=1 ; i<=(nbChannels-1)/2 ; i++)
        {
            std::copy(ACNsig[doneChannels].begin(), ACNsig[doneChannels].end(), std::back_inserter(FuMAsig[middleIdx+i]));
            std::copy(ACNsig[doneChannels+1].begin(), ACNsig[doneChannels+1].end(), std::back_inserter(FuMAsig[middleIdx-i]));
            doneChannels += 2;
        }

        prevNbChannels= nbChannels;
    }

    for(int i=0 ; i<min(nSH, max_FuMA_nsh) ; i++)
        for(int j=0 ; j<nSamples ; j++)
            in[i][j] = FuMAsig[i][j];
}


void convertFUMAtoACN(std::vector<std::vector<float>>& in, int order, int nSamples)
{
    using namespace std;

    if(order <= 0)
        return ;

    int nSH = (order+1)*(order+1);

    vector<vector<float>> ACNsig(nSH, vector<float>(nSamples));
    vector<vector<float>> FuMAsig(nSH, vector<float>(nSamples));

    for(int i=0 ; i<nSH ; i++)
        for(int j=0 ; j<nSamples ; j++)
        {
            ACNsig[i][j] = in[i][j];
            FuMAsig[i][j] = in[i][j];
            //in[i][j] = 0.0f;
        }

    ACNsig[1].swap(ACNsig[2]);
    ACNsig[1].swap(ACNsig[3]);

    int doneChannels = 4;
    int prevNbChannels = 3;

    while(doneChannels < min(nSH, max_FuMA_nsh))
    {

        int nbChannels = prevNbChannels+2;
        int middleIdx = doneChannels + (nbChannels-1)/2;

        std::copy(FuMAsig[middleIdx].begin(), FuMAsig[middleIdx].end(), std::back_inserter(ACNsig[doneChannels]));
        ++doneChannels;

        for(int i=1 ; i<=(nbChannels-1)/2 ; i++)
        {
            std::copy(FuMAsig[middleIdx+i].begin(), FuMAsig[middleIdx+i].end(), std::back_inserter(ACNsig[doneChannels]));
            std::copy(FuMAsig[middleIdx-i].begin(), FuMAsig[middleIdx-i].end(), std::back_inserter(ACNsig[doneChannels+1]));

            doneChannels += 2;
        }

        prevNbChannels+= nbChannels;
    }

    for(int i=0 ; i < min(nSH, max_FuMA_nsh) ; i++)
        for(int j=0 ; j<nSamples ; j++)
            in[i][j] = ACNsig[i][j];
}



















