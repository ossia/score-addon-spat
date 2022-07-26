#include <Spat/_VbapMethods.hpp>

static constexpr float deg_to_rad = M_PI / 180.f;

static int cmp_desc_double(const void* a, const void* b)
{
  sort_double* a1 = (sort_double*)a;
  sort_double* a2 = (sort_double*)b;
  if ((*a1).val > (*a2).val)
    return -1;
  else if ((*a1).val < (*a2).val)
    return 1;
  else
    return 0;
}

static int cmp_asc_double(const void* a, const void* b)
{
  sort_double* a1 = (sort_double*)a;
  sort_double* a2 = (sort_double*)b;
  if ((*a1).val < (*a2).val)
    return -1;
  else if ((*a1).val > (*a2).val)
    return 1;
  else
    return 0;
}

void sortf(
    std::vector<double>& in_vec,
    std::vector<int> new_indices,
    int len,
    int descendFLAG)
{
  int i;
  sort_double* data = (sort_double*)alloca(sizeof(sort_double) * (len));

  for (i = 0; i < len; i++)
  {
    data[i].val = in_vec[i];
    data[i].idx = i;
  }

  if (descendFLAG)
    std::qsort(data, len, sizeof(sort_double), cmp_desc_double);
  else
    std::qsort(data, len, sizeof(sort_double), cmp_asc_double);

  for (i = 0; i < len; i++)
    new_indices[i] = data[i].idx;
}

void invertLsMtx2D(
    std::vector<double> U_spkr /* L x 2 */,
    std::vector<int> ls_pairs /* N_group x 2 */,
    int N_pairs,
    std::vector<double>& layoutInvMtx /* N_group x 4 */
)
{
  float tempGroup[4];

  layoutInvMtx.resize(N_pairs * 4);
  for (int n = 0; n < N_pairs; n++)
  {
    /* get the unit vectors for the current group */
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        tempGroup[j * 2 + i] = U_spkr[ls_pairs[n * 2 + i] * 2 + j]; /* ^T */

    /* get inverse of current group */
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        layoutInvMtx[j * 2 + i + n * 4] = tempGroup[i * 2 + j];
  }
}

int findLsPairs(std::vector<double> ls_dirs_deg, int L, std::vector<int>& out_pairs)
{
  std::vector<double> ls_dirs_deg_tmp(L);
  std::vector<int> idx_sorted(L);

  for (int n = 0; n < L; n++)
    ls_dirs_deg_tmp[n] = ls_dirs_deg[n * 2];

  /* find the loudspeaker pairs by sorting the angles */
  sortf(ls_dirs_deg_tmp, idx_sorted, L, 0);

  idx_sorted.resize(L + 1);
  idx_sorted[L] = idx_sorted[0];

  out_pairs.resize(L * 2);

  for (int n = 0; n < L; n++)
  {
    out_pairs[n * 2] = idx_sorted[n];
    out_pairs[n * 2 + 1] = idx_sorted[n + 1];
  }

  return L;
}

void Vbap2D(
    int ls_num,
    std::vector<int> ls_pairs,
    int N_pairs,
    std::vector<double> layoutInvMtx,
    std::vector<double>& GainMtx)
{
  std::vector<double> gains(ls_num);
  GainMtx.resize(ls_num);

  double src_dir = 0.;
  double azi_rad = src_dir * deg_to_rad;

  double cosAzi = cos(azi_rad);
  double sinAzi = sin(azi_rad);

  for (int i = 0; i < N_pairs; i++)
  {
    double g_tmp[2], ls_invMtx_s[2];

    for (int j = 0; j < 2; j++)
      ls_invMtx_s[j] = layoutInvMtx[i * 4 + j];
    g_tmp[0] = ls_invMtx_s[0] * cosAzi + ls_invMtx_s[1] * sinAzi;

    for (int j = 0; j < 2; j++)
      ls_invMtx_s[j] = layoutInvMtx[i * 4 + j + 2];
    g_tmp[1] = ls_invMtx_s[0] * cosAzi + ls_invMtx_s[1] * sinAzi;

    double min_val = 2.23e13;
    double g_tmp_rms = 0.0;

    for (int j = 0; j < 2; j++)
    {
      min_val = std::min(min_val, g_tmp[j]);
      g_tmp_rms += powf(g_tmp[j], 2.0f);
    }

    g_tmp_rms = sqrtf(g_tmp_rms);

    if (min_val > -0.001)
    {
      for (int j = 0; j < 2; j++)
        gains[ls_pairs[i * 2 + j]] = g_tmp[j] / g_tmp_rms;
    }
  }

  double gains_rms = 0.0;
  for (int i = 0; i < ls_num; i++)
    gains_rms += powf(gains[i], 2.0f);
  gains_rms = sqrtf(gains_rms);
  for (int i = 0; i < ls_num; i++)
    GainMtx[i] = std::max(gains[i] / gains_rms, 0.0);
}

void generateVBAPgainTable2D(
    std::vector<double>& ls_dirs_deg,
    int L,
    int az_res_deg,
    std::vector<double>& gtable /* N_gtable x L  */,
    //int* N_gtable,
    int* nPairs)
{
  std::vector<int> out_pairs;

  /* compute directions for the grid */
  //int N_azi = (int)((360.0f/(float)az_res_deg) + 1.5f);
  //double* src_dirs = (double*) alloca(sizeof(double)*N_azi);

  //for(int fi = -180, i = 0; i<N_azi; fi+=az_res_deg, i++)
  //    src_dirs[i] = (float) fi;

  std::fill(out_pairs.begin(), out_pairs.end(), 0.);
  int numOutPairs = findLsPairs(ls_dirs_deg, L, out_pairs);
  std::vector<double> layoutInvMtx(numOutPairs * 4), ls_vertices(L * 2);

  for (int i = 0; i < L; i++)
  {
    ls_vertices[i * 2 + 0] = cos(ls_dirs_deg[i * 2] * deg_to_rad);
    ls_vertices[i * 2 + 1] = sin(ls_dirs_deg[i * 2] * deg_to_rad);
  }

  /* Invert matrix */
  invertLsMtx2D(ls_vertices, out_pairs, numOutPairs, layoutInvMtx);

  /* Calculate VBAP gains for each source position */
  //int N_points = N_azi;
  Vbap2D(L, out_pairs, numOutPairs, layoutInvMtx, gtable);
  (*nPairs) = numOutPairs;
  //(*N_gtable) = N_points;
}
