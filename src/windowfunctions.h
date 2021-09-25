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

#ifndef WINDOWFUNCTIONS_H
#define WINDOWFUNCTIONS_H

#include "constants.h"

namespace KeyFinder {

class WindowFunction {
public:
    [[nodiscard]] static auto window(TemporalWindowT windowType, int n, int N) -> float;
    [[nodiscard]] static auto gaussianWindow(int n, int N, float sigma) -> float;
    [[nodiscard]] static auto convolve(const std::vector<float>& input, const std::vector<float>& window) -> std::vector<float>;
};

}

#endif
