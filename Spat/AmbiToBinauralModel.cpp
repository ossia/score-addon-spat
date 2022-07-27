#include <Spat/AmbiToBinauralModel.hpp>

namespace Spat
{
void AmbiToBinaural::operator()(halp::tick t)
{
    if (inputs.audio.channels <= 0 || nSamples == 0 || outputs.audio.channels < 2)
      return;

    // Process the input buffer
    order = inputs.order;
    nSH = (order + 1) * (order + 1);

    yaw = inputs.yaw * deg_to_rad;
    pitch = inputs.pitch * deg_to_rad;
    roll = inputs.roll * deg_to_rad;

    float** in = inputs.audio.samples;
    float** out = outputs.audio.samples;

    /*for (int i = 0; i < nSH; i++)
        for (int j = 0; j < nSamples; j++)
            inFrame[i][j] = in[i][j];*/

    float Rxyz[3][3];

    // convert input signal into RSH
    azi = inputs.azi;
    elev = inputs.elev;

    getRSH(order, azi, elev, y);

    yawPitchRoll2Rzyx(yaw, pitch, roll, Rxyz);
    getSHrotMtxReal(Rxyz, order, M_rot_tmp, max_nsh*max_nsh);

    std::complex<float>** specs = (std::complex<float>**) alloca(sizeof(std::complex<float>)*(nSH*nSamples));
    float* ambiVec = (float*) alloca(sizeof(float)*nSamples);

    for (int i = 0; i < nSH; i++)
    {
        for (int j = 0; j < nSamples; j++)
            ambiVec[j] = y[i] * in[0][j];
        specs[i] = FFT.execute(ambiVec, nSamples);
    }

    //algo ici
    /*
    getBinauralAmbiDecoderMtx(pars->hrtf_fb, pars->hrir_dirs_deg, pars->N_hrir_dirs, HYBRID_BANDS,
                              BINAURAL_DECODER_MAGLS, order, pData->freqVector, pars->itds_s, pars->weights,
                              pData->enableDiffuseMatching, pData->enableMaxRE, decMtx);

    for(band=0; band<HYBRID_BANDS; band++)
            for(i=0; i<NUM_EARS; i++)
                for(j=0; j<nSH; j++)
                    pars->M_dec[band][i][j] = decMtx[band*NUM_EARS*nSH + i*nSH + j];

    for(int i = 0; i < nSamples; i++) {
        cblas_cgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, NUM_EARS, TIME_SLOTS, nSH, &calpha,
                    pars->M_dec[i], MAX_NUM_SH_SIGNALS,
                    FLATTEN2D(pData->SHframeTF[i]), TIME_SLOTS, &cbeta,
                    FLATTEN2D(pData->binframeTF[i]), TIME_SLOTS);
        }*/

    for (int i = 0 ; i < nSH ; i++)
    {
        out[i] = FFT.execute(specs[i], nSamples);
        for(int j = 0 ; j < nSamples ; j++)
            out[i][j] /= nSamples ;     // normalisation
    }
  }
}

