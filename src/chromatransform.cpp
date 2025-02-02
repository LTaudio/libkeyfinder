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

#include "chromatransform.h"

namespace KeyFinder {

ChromaTransform::ChromaTransform(unsigned int inFrameRate)
{

    frameRate = inFrameRate;
    if (frameRate < 1) {
        throw Exception("Frame rate must be > 0");
    }

    if (getLastFrequency() > frameRate / 2.0) {
        throw Exception("Analysis frequencies over Nyquist");
    }

    if (frameRate / (float)FFTFRAMESIZE > (getFrequencyOfBand(1) - getFrequencyOfBand(0))) {
        throw Exception("Insufficient low-end resolution");
    }

    chromaBandFftBinOffsets.resize(BANDS, 0);
    directSpectralKernel.resize(BANDS, std::vector<float>(0, 0.0));

    float myQFactor = DIRECTSKSTRETCH * (pow(2, (1.0 / SEMITONES)) - 1);

    for (unsigned int i = 0; i < BANDS; i++) {

        float centreOfWindow = getFrequencyOfBand(i) * FFTFRAMESIZE / inFrameRate;
        float widthOfWindow = centreOfWindow * myQFactor;
        float beginningOfWindow = centreOfWindow - (widthOfWindow / 2);
        float endOfWindow = beginningOfWindow + widthOfWindow;

        float sumOfCoefficients = 0.0;

        chromaBandFftBinOffsets[i] = ceil(beginningOfWindow); // first useful fft bin
        for (unsigned int fftBin = chromaBandFftBinOffsets[i]; fftBin <= floor(endOfWindow); fftBin++) {
            float coefficient = kernelWindow(fftBin - beginningOfWindow, widthOfWindow);
            sumOfCoefficients += coefficient;
            directSpectralKernel[i].push_back(coefficient);
        }

        // normalisation by sum of coefficients and frequency of bin; models CQT very closely
        for (unsigned int j = 0; j < directSpectralKernel[i].size(); j++) {
            directSpectralKernel[i][j] = directSpectralKernel[i][j] / sumOfCoefficients * getFrequencyOfBand(i);
        }
    }
}

auto ChromaTransform::kernelWindow(float n, float nn) -> float
{
    // discretely sampled continuous function, but different to other window functions
    return 1.0 - cos((2 * PI * n) / nn);
}

auto ChromaTransform::chromaVector(const FftAdapter* const fftAdapter) const -> std::vector<float>
{
    std::vector<float> chromaVector(BANDS);
    for (unsigned int i = 0; i < BANDS; i++) {
        float sum = 0.0;
        for (unsigned int j = 0; j < directSpectralKernel[i].size(); j++) {
            float magnitude = fftAdapter->getOutputMagnitude(chromaBandFftBinOffsets[i] + j);
            sum += (magnitude * directSpectralKernel[i][j]);
        }
        chromaVector[i] = sum;
    }
    return chromaVector;
}

}
