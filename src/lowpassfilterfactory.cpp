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

#include "lowpassfilterfactory.h"

namespace KeyFinder {

LowPassFilterFactory::LowPassFilterWrapper::LowPassFilterWrapper(unsigned int inOrder, unsigned int inFrameRate, double inCornerFrequency, unsigned int inFftFrameSize, const LowPassFilter* const inLowPassFilter)
{
    order_ = inOrder;
    frameRate_ = inFrameRate;
    cornerFrequency_ = inCornerFrequency;
    fftFrameSize_ = inFftFrameSize;
    lowPassFilter_ = inLowPassFilter;
}

LowPassFilterFactory::LowPassFilterWrapper::~LowPassFilterWrapper()
{
    delete lowPassFilter_;
}

auto LowPassFilterFactory::LowPassFilterWrapper::getLowPassFilter() const -> const LowPassFilter*
{
    return lowPassFilter_;
}

auto LowPassFilterFactory::LowPassFilterWrapper::getOrder() const -> unsigned int
{
    return order_;
}

auto LowPassFilterFactory::LowPassFilterWrapper::getFrameRate() const -> unsigned int
{
    return frameRate_;
}

auto LowPassFilterFactory::LowPassFilterWrapper::getCornerFrequency() const -> double
{
    return cornerFrequency_;
}

auto LowPassFilterFactory::LowPassFilterWrapper::getFftFrameSize() const -> unsigned int
{
    return fftFrameSize_;
}

LowPassFilterFactory::LowPassFilterFactory()
    : lowPassFilters_(0)
{
}

LowPassFilterFactory::~LowPassFilterFactory()
{
    for (auto& lowPassFilter : lowPassFilters_) {
        delete lowPassFilter;
    }
}

auto LowPassFilterFactory::getLowPassFilter(unsigned int inOrder, unsigned int inFrameRate, double inCornerFrequency, unsigned int inFftFrameSize) -> const LowPassFilter*
{
    for (auto* wrapper : lowPassFilters_) {
        if (wrapper->getOrder() == inOrder && wrapper->getFrameRate() == inFrameRate && wrapper->getCornerFrequency() == inCornerFrequency && wrapper->getFftFrameSize() == inFftFrameSize) {
            return wrapper->getLowPassFilter();
        }
    }
    lowPassFilterFactoryMutex_.lock();
    auto* lpf = new LowPassFilter(inOrder, inFrameRate, inCornerFrequency, inFftFrameSize);
    lowPassFilters_.push_back(new LowPassFilterWrapper(inOrder, inFrameRate, inCornerFrequency, inFftFrameSize, lpf));
    unsigned int newLowPassFilterIndex = lowPassFilters_.size() - 1;
    lowPassFilterFactoryMutex_.unlock();
    return lowPassFilters_[newLowPassFilterIndex]->getLowPassFilter();
}

}
