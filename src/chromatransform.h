/*************************************************************************

  Copyright 2011-2015 Ibrahim Sha'ath

  This file is part of LibKeyFinder.

  LibKeyFinder is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  LibKeyFinder is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with LibKeyFinder.  If not, see <http://www.gnu.org/licenses/>.

*************************************************************************/

#ifndef CHROMATRANSFORM_H
#define CHROMATRANSFORM_H

#include "constants.h"
#include "fftadapter.h"

namespace KeyFinder {

class ChromaTransform {
public:
    ChromaTransform(unsigned int frameRate);
    auto chromaVector(const FftAdapter* fft) const -> std::vector<float>;

protected:
    unsigned int frameRate;
    std::vector<std::vector<float>> directSpectralKernel;
    std::vector<unsigned int> chromaBandFftBinOffsets;
    [[nodiscard]] static auto kernelWindow(float n, float nn) -> float;
};

}

#endif
