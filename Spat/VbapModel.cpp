#include <Spat/VbapModel.hpp>

namespace Spat{
void Vbap::operator()(halp::tick t)
{
    if (inputs.audio.channels <= 0 || nSamples == 0 || outputs.audio.channels < 2)
      return;

    double** in = inputs.audio.samples;
    double** out = outputs.audio.samples;

    // convert input signal into RSH
    azi = inputs.azi;
    elev = inputs.elev;

    std::complex<double>** inFrameSpec = (std::complex<double>**) alloca(sizeof(std::complex<double>)*(nChannels*nSamples));
    double* inFrame = (double*) alloca(sizeof(double)*nSamples);

    std::complex<double>** outFrameSpec = (std::complex<double>**) alloca(sizeof(std::complex<double>)*(nChannels*nSamples));
    double* outFrame = (double*) alloca(sizeof(double)*nSamples);

    for (int i = 0; i < nChannels; i++)
    {
        for (int j = 0; j < nSamples; j++)
            inFrame[j] = in[0][j];
        inFrameSpec[i] = FFT.execute(inFrame, nSamples);
    }

    /*
    float Rxyz[3][3];

    float src_dirs_deg[2] = {0.f, 0.f};
    float src_dirs_xyz[3];
    float src_dirs_rot_xyz[3];
    // Rotate source directions
    yawPitchRoll2Rzyx (yaw, pitch, roll, Rxyz);

    src_dirs_xyz[0] = cos(deg_to_rad * src_dirs_deg[1]) * cos(deg_to_rad * src_dirs_deg[0]);
    src_dirs_xyz[1] = cos(deg_to_rad * src_dirs_deg[1]) * sin(deg_to_rad * src_dirs_deg[0]);
    src_dirs_xyz[2] = sin(deg_to_rad * src_dirs_deg[1]);

    for(int i = 0 ; i < 3 ; i++)
    {
        src_dirs_rot_xyz[i] = 0;
        for(int j = 0 ; j<3 ; j++)
            src_dirs_rot_xyz[i] += src_dirs_xyz[i] * Rxyz[i][j];
    }

    hypotxy = sqrtf(powf(src_dirs_rot_xyz[i][0], 2.0f) + powf(src_dirs_rot_xyz[i][1], 2.0f));
    src_dirs_rot_deg[0] = rad_to_deg * atan2f(src_dirs_rot_xyz[1], src_dirs_rot_xyz[0]);    //azi
    src_dirs_rot_deg[1] = rad_to_deg * atan2f(src_dirs_rot_xyz[2], hypotxy);                //elev
    */

    float room = inputs.room/2.f;
    float azi = inputs.azi;
    int az_res_deg = 1;

    // TODO : generateVBAPgainTable2D(loudpkrs_dirs_deg, nChannels, az_res_deg,
    //                                &vbap_gtable, nChannels);

    int HYBRID_BANDS = 0, TIME_SLOTS = 0; // TODO
    // recalculate frequency dependent panning gains
    int idx2D = (int)((mod(azi+180.0f,360.0f))+0.5f);
    for (int ls = 0; ls < nChannels; ls++)
        gains2D[ls] = vbap_gtable[idx2D*nChannels+ls];
    for (int band = 0; band < HYBRID_BANDS; band++){
        // apply pValue per frequency
        double pv_f = 0; // TODO : pValue[band];
        if(pv_f != 2.0f){
            double gains2D_sum_pvf = 0.f;
            for (int ls = 0; ls < nChannels; ls++)
                gains2D_sum_pvf += pow(std::max(gains2D[ls], 0.0), pv_f);
            gains2D_sum_pvf = powf(gains2D_sum_pvf, 1.0f/(pv_f+2.23e-9f));
            for (int ls = 0; ls < nChannels; ls++)
            {
                std::complex<double> gain(gains2D[ls] / (gains2D_sum_pvf+2.23e-9f), 0.0f);
                G_src[band][ls] = gain;
            }
        }
        else
            for (int ls = 0; ls < nChannels; ls++)
            {
                std::complex<double> gain(gains2D[ls], 0.0f);
                G_src[band][ls] = gain;
            }
    }

    // apply panning gains
    for (int band = 0; band < HYBRID_BANDS; band++){
        for (int ls = 0; ls < nChannels; ls++)
            for (int t = 0; t < TIME_SLOTS; t++)
                std::cout << "TODO" << std::endl;
                // TODO : outFrameSpec[band][ls][t] = ccaddf(pData->outFrameSpec[band][ls][t], ccmulf(pData->inFrameSpec[band][ch][t], pData->G_src[band][ch][ls]));
    }

    for (int i = 0 ; i < nChannels ; i++)
    {
        out[i] = FFT.execute(outFrameSpec[i], nSamples);
        for(int j = 0 ; j < nSamples ; j++)
            out[i][j] /= nSamples ;     // normalisation
    }
}
}
