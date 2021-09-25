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

#include "windowfunctions.h"

namespace KeyFinder {

auto WindowFunction::window(TemporalWindowT windowType, int n, int nn) -> float
{
    switch (windowType) {
    case WINDOW_BLACKMAN:
        return 0.42f - (0.5f * cos((2.0f * PI * n) / (nn - 1))) + (0.08f * cos((4.0f * PI * n) / (nn - 1.0f)));
    default:
        // This should be unreachable code, but just in case fall back to hamming window.
        // fall through
    case WINDOW_HAMMING:
        return 0.54f - (0.46f * cos((2 * PI * n) / (nn - 1)));
    }
}

auto WindowFunction::gaussianWindow(int n, int nn, float sigma) -> float
{
    return exp(-1 * (pow(n - (nn / 2), 2) / (2 * sigma * sigma)));
}

auto WindowFunction::convolve(const std::vector<float>& input, const std::vector<float>& window) -> std::vector<float>
{

    unsigned int inputSize = input.size();
    unsigned int padding = window.size() / 2;
    std::vector<float> convolved(inputSize, 0.0);

    // TODO: this implements zero padding for boundary effects, write something mean-based later.
    for (unsigned int sample = 0; sample < inputSize; sample++) {
        float convolution = 0.0;
        for (unsigned int k = 0; k < window.size(); k++) {
            int frm = (signed)sample - (signed)padding + (signed)k;
            if (frm >= 0 && frm < (signed)inputSize) {
                // don't run off either end
                convolution += input[frm] * window[k] / window.size();
            }
        }
        convolved[sample] = convolution;
    }
    return convolved;
}

}
