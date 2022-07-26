#pragma once

#include <cmath>

#include <algorithm>
#include <iostream>
#include <span>

typedef struct
{
  double val;
  int idx;
} sort_double;

static int cmp_desc_double(const void* a, const void* b);
static int cmp_asc_double(const void* a, const void* b);

void sortf(
    std::vector<double>& in_vec,
    std::vector<int> new_indices,
    int len,
    int descendFLAG);

void invertLsMtx2D(
    std::vector<double> U_spkr /* L x 2 */,
    std::vector<int> ls_pairs /* N_group x 2 */,
    int N_pairs,
    std::vector<double>& layoutInvMtx /* N_group x 4 */
);

int findLsPairs(std::vector<double> ls_dirs_deg, int L, std::vector<int>& out_pairs);

void Vbap2D(
    int ls_num,
    std::vector<int> ls_pairs,
    int N_pairs,
    std::vector<double> layoutInvMtx,
    std::vector<double>& GainMtx);

void generateVBAPgainTable2D(
    std::vector<double>& ls_dirs_deg,
    int L,
    int az_res_deg,
    std::vector<double>& gtable /* N_gtable x L  */,
    //int* N_gtable,
    int* nPairs);
