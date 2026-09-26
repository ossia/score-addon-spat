/*
 This file is part of SpatGRIS.

 Developers: Gaël Lane Lépine, Samuel Béland, Olivier Bélanger, Nicolas Masson

 SpatGRIS is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 SpatGRIS is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with SpatGRIS.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * Functions for 3D VBAP processing based on work by Ville Pulkki.
 * (c) Ville Pulkki - 2.2.1999 Helsinki University of Technology.
 * Updated by belangeo, 2017.
 * Updated by Samuel Béland, 2021.
 */

#pragma once

#include <Gris/Algo/SpeakerSetup.hpp>
#include <Gris/Algo/Types.hpp>

#include <array>
#include <cstddef>
#include <memory>
#include <vector>

namespace Gris
{
struct SourceData;

using InverseMatrix = std::array<float, 9>;

struct SpeakerSet
{
  std::array<output_patch_t, 3> speakerNos;
  InverseMatrix invMx;
  std::array<float, 3> setGains;
  float smallestWt;
  int negGAm;
};

struct VbapData
{
  std::array<output_patch_t, MAX_NUM_SPEAKERS> outputPatches{};
  std::array<float, MAX_NUM_SPEAKERS> gainsSmoothing{};
  std::size_t dimension{};
  std::vector<SpeakerSet> speakerSets{};
  int numOutputPatches{};
  int numSpeakers{};
  Position direction{};
  CartesianVector spreadingVector{};
};

std::unique_ptr<VbapData> vbapInit(
    std::array<Position, MAX_NUM_SPEAKERS>& speakers, int count, int dimensions,
    std::array<output_patch_t, MAX_NUM_SPEAKERS> const& outputPatches);

void vbapCompute(
    SourceData const& source, SpeakersSpatGains& gains, VbapData& data) noexcept;

std::vector<Triplet> vbapExtractTriplets(VbapData const& data);

}
