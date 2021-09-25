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

#include "temporalwindowfactory.h"

namespace KeyFinder {

TemporalWindowFactory::TemporalWindowWrapper::TemporalWindowWrapper(unsigned int frameSize)
{
    WindowFunction win;
    temporalWindow_.resize(frameSize);
    auto twIt = temporalWindow_.begin();
    for (unsigned int i = 0; i < frameSize; i++) {
        *twIt = win.window(WINDOW_BLACKMAN, i, frameSize);
        std::advance(twIt, 1);
    }
}

auto TemporalWindowFactory::TemporalWindowWrapper::getFrameSize() const -> unsigned int
{
    return temporalWindow_.size();
}

auto TemporalWindowFactory::TemporalWindowWrapper::getTemporalWindow() const -> const std::vector<float>*
{
    return &temporalWindow_;
}

TemporalWindowFactory::TemporalWindowFactory()
    : temporalWindows_(0)
{
}

TemporalWindowFactory::~TemporalWindowFactory()
{
    for (auto& temporalWindow : temporalWindows_) {
        delete temporalWindow;
    }
}

auto TemporalWindowFactory::getTemporalWindow(unsigned int frameSize) -> const std::vector<float>*
{
    for (auto* wrapper : temporalWindows_) {
        if (wrapper->getFrameSize() == frameSize) {
            return wrapper->getTemporalWindow();
        }
    }
    temporalWindowFactoryMutex_.lock();
    temporalWindows_.push_back(new TemporalWindowWrapper(frameSize));
    unsigned int newTemporalWindowIndex = temporalWindows_.size() - 1;
    temporalWindowFactoryMutex_.unlock();
    return temporalWindows_[newTemporalWindowIndex]->getTemporalWindow();
}

}
