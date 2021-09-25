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

#ifndef LOWPASSFILTERFACTORY_H
#define LOWPASSFILTERFACTORY_H

#include "constants.h"
#include "lowpassfilter.h"

namespace KeyFinder {

class LowPassFilterFactory {
public:
    LowPassFilterFactory();
    ~LowPassFilterFactory();
    auto getLowPassFilter(unsigned int order, unsigned int frameRate, float cornerFrequency, unsigned int fftFrameSize) -> const LowPassFilter*;

private:
    class LowPassFilterWrapper;
    std::vector<LowPassFilterWrapper*> lowPassFilters_;
    std::mutex lowPassFilterFactoryMutex_;
};

class LowPassFilterFactory::LowPassFilterWrapper {
public:
    LowPassFilterWrapper(unsigned int order, unsigned int frameRate, float cornerFrequency, unsigned int fftFrameSize, const LowPassFilter* filter);
    ~LowPassFilterWrapper();
    [[nodiscard]] auto getLowPassFilter() const -> const LowPassFilter*;
    [[nodiscard]] auto getOrder() const -> unsigned int;
    [[nodiscard]] auto getFrameRate() const -> unsigned int;
    [[nodiscard]] auto getCornerFrequency() const -> float;
    [[nodiscard]] auto getFftFrameSize() const -> unsigned int;

private:
    unsigned int order_;
    unsigned int frameRate_;
    float cornerFrequency_;
    unsigned int fftFrameSize_;
    const LowPassFilter* lowPassFilter_;
};

}

#endif
