#include "AmbisonicMethods.hpp"

void yawPitchRoll2Rzyx
(
    float yaw,
    float pitch,
    float roll,
    int rollPitchYawFLAG,
    float R[3][3]
)
{
    float R1[3][3], R2[3][3], R3[3][3], Rtmp[3][3];

    R1[0][0] = cosf(yaw);
    R1[0][1] = sinf(yaw);
    R1[0][2] = 0.0f;
    R1[1][0] = -sinf(yaw);
    R1[1][1] = cosf(yaw);
    R1[1][2] = 0.0f;
    R1[2][0] = 0.0f;
    R1[2][1] = 0.0f;
    R1[2][2] = 1.0f;

    R2[0][0] = cosf(pitch);
    R2[0][1] = 0.0f;
    R2[0][2] = -sinf(pitch);
    R2[1][0] = 0.0f;
    R2[1][1] = 1.0f;
    R2[1][2] = 0.0f;
    R2[2][0] = sinf(pitch);
    R2[2][1] = 0.0f;
    R2[2][2] = cosf(pitch);

    R3[0][0] = 1.0f;
    R3[0][1] = 0.0f;
    R3[0][2] = 0.0f;
    R3[1][0] = 0.0f;
    R3[1][1] = cosf(roll);
    R3[1][2] = sinf(roll);
    R3[2][0] = 0.0f;
    R3[2][1] = -sinf(roll);
    R3[2][2] = cosf(roll);

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
float getP
(
    int M,
    int i,
    int l,
    int a,
    int b,
    float R_1[3][3],
    std::vector<float> R_lm1
)
{
    float ret, ri1, rim1, ri0;

    ri1 = R_1[i+1][2];
    rim1 = R_1[i+1][0];
    ri0 = R_1[i+1][1];

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
float getU
(
    int M,
    int l,
    int m,
    int n,
    float R_1[3][3],
    std::vector<float> R_lm1
)
{
    return getP(M, 0, l, m, n, R_1, R_lm1);
}

/* Ivanic, J., Ruedenberg, K. (1998). Rotation Matrices for Real Spherical Harmonics. Direct Determination
 * by Recursion Page: Additions and Corrections. Journal of Physical Chemistry A, 102(45), 9099?9100. */
float getV
(
    int M,
    int l,
    int m,
    int n,
    float R_1[3][3],
    std::vector<float> R_lm1
)
{
    int d;
    float ret, p0, p1;

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
            ret = p0*sqrtf(1.0f + d) - p1*(1.0f - d);
        }
        else {
            d = m == -1 ? 1 : 0;
            p0 = getP(M, 1, l, m + 1, n, R_1, R_lm1);
            p1 = getP(M, -1, l, -m - 1, n, R_1, R_lm1);
            ret = p0*(1.0f - (float)d) + p1*sqrtf(1.0f + (float)d);
        }
    }

    return ret;
}

/* Ivanic, J., Ruedenberg, K. (1998). Rotation Matrices for Real Spherical Harmonics. Direct Determination
 * by Recursion Page: Additions and Corrections. Journal of Physical Chemistry A, 102(45), 9099?9100. */
float getW
(
    int M,
    int l,
    int m,
    int n,
    float R_1[3][3],
    std::vector<float> R_lm1
)
{
    float ret, p0, p1;
    ret = 0.0f;

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

std::vector<float> getSHrotMtxReal
(
    float Rxyz[3][3],
    //float* RotMtx/*(L+1)^2 x (L+1)^2 */,
    int L,
    int size
)
{
    int M=(L+1)*(L+1);
    int d, bandIdx, denom;
    float u, v, w;
    float R_1[3][3];//, R_lm1[M], R_l[M];
    std::vector<float> R_lm1(M), R_l(M);

    //memset(RotMtx, 0, M*M*sizeof(float));
    std::vector<float> RotMtx(size);
    std::fill(RotMtx.begin(), RotMtx.end(), 0);

    /* zeroth-band (l=0) is invariant to rotation */
    RotMtx[0] = 1;

    /* the first band (l=1) is directly related to the rotation matrix */
    R_1[0][0] = Rxyz[1][1];
    R_1[0][1] = Rxyz[1][2];
    R_1[0][2] = Rxyz[1][0];
    R_1[1][0] = Rxyz[2][1];
    R_1[1][1] = Rxyz[2][2];
    R_1[1][2] = Rxyz[2][0];
    R_1[2][0] = Rxyz[0][1];
    R_1[2][1] = Rxyz[0][2];
    R_1[2][2] = Rxyz[0][0];
    for (int i=1; i<4; i++){
        R_lm1[(i-1)*M+0] = R_1[i-1][0];
        R_lm1[(i-1)*M+1] = R_1[i-1][1];
        R_lm1[(i-1)*M+2] = R_1[i-1][2];
        for (int j=1; j<4; j++)
            RotMtx[i*M+j] = R_1[i-1][j-1];
    }

    /* compute rotation matrix of each subsequent band recursively */
    bandIdx = 4;
    std::fill(R_l.begin(), R_l.end(), 0);
    for(int l = 2; l<=L; l++){
        for(int m=-l; m<=l; m++){
            for(int n=-l; n<=l; n++){
                /* compute u,v,w terms of Eq.8.1 (Table I) */
                d = m == 0 ? 1 : 0; /* the delta function d_m0 */
                denom = abs(n) == l ? (2*l)*(2*l-1) : (l*l-n*n);
                u = sqrtf( (float)((l*l-m*m)) /  (float)denom);
                v = sqrtf( (float)((1+d)*(l+abs(m)-1)*(l+abs(m))) /  (float)denom) * (float)(1-2*d)*0.5f;
                w = sqrtf( (float)((l-abs(m)-1)*(l-abs(m))) / (float)denom) * (float)(1-d)*(-0.5f);

                /* computes Eq.8.1 */
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
            std::copy(R_lm1.begin(), R_lm1.end(), R_l.begin());

        bandIdx += 2*l+1;
    }

    return RotMtx;
}
