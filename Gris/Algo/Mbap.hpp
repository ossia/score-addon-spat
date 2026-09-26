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

/**
 * Matrix-Based Amplitude Panning framework.
 *
 * MBAP (Matrix-Based Amplitude Panning) is a framework
 * to do 3-D sound spatialization. It uses a pre-computed
 * gain matrix to perform the spatialization of the sources very
 * efficiently.
 *
 * author : Gaël Lane Lépine, 2022
 * based on lbap from Olivier Belanger
 *
 */

#pragma once

#include <Gris/Algo/SpeakerSetup.hpp>
#include <Gris/Algo/Types.hpp>

#include <array>
#include <cstddef>
#include <vector>

namespace Gris
{
struct SpeakerData;

static auto constexpr MBAP_MATRIX_SIZE = 64;
using matrix_t = std::array<
    std::array<std::array<float, MBAP_MATRIX_SIZE + 1>, MBAP_MATRIX_SIZE + 1>,
    MBAP_MATRIX_SIZE + 1>;

struct MbapSpeaker
{
  Position position{};
  output_patch_t outputPatch{};
};

struct MbapField
{
  std::vector<output_patch_t> outputOrder;
  float fieldExponent;
  std::vector<matrix_t> amplitudeMatrix;
  std::vector<Position> speakerPositions;

  [[nodiscard]] size_t getNumSpeakers() const;
  void reset();
};

MbapField mbapInit(SpeakersData const& speakers);

void mbap(SourceData const& source, SpeakersSpatGains& gains, MbapField const& field);

}
