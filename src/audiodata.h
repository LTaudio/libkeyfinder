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

#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H

#include "constants.h"

namespace KeyFinder {

class AudioData {
public:
    AudioData();

    [[nodiscard]] auto getChannels() const -> unsigned int;
    [[nodiscard]] auto getFrameRate() const -> unsigned int;
    [[nodiscard]] auto getSample(unsigned int index) const -> float;
    [[nodiscard]] auto getSampleByFrame(unsigned int frame, unsigned int channel) const -> float;
    [[nodiscard]] auto getSampleAtReadIterator() const -> float;
    [[nodiscard]] auto getSampleCount() const -> unsigned int;
    [[nodiscard]] auto getFrameCount() const -> unsigned int;

    void setChannels(unsigned int inChannels);
    void setFrameRate(unsigned int inFrameRate);
    void setSample(unsigned int index, float value);
    void setSampleByFrame(unsigned int frame, unsigned int channels, float value);
    void setSampleAtWriteIterator(float value);
    void addToSampleCount(unsigned int inSamples);
    void addToFrameCount(unsigned int inFrames);

    void advanceReadIterator(unsigned int by = 1);
    void advanceWriteIterator(unsigned int by = 1);
    [[nodiscard]] auto readIteratorWithinUpperBound() const -> bool;
    [[nodiscard]] auto writeIteratorWithinUpperBound() const -> bool;
    void resetIterators();

    void append(const AudioData& that);
    void prepend(const AudioData& that);
    void discardFramesFromFront(unsigned int discardFrameCount);
    void reduceToMono();
    void downsample(unsigned int factor, bool shortcut = true);
    auto sliceSamplesFromBack(unsigned int sliceSampleCount) -> AudioData*;

private:
    std::deque<float> samples_;
    unsigned int channels_ { 0 };
    unsigned int frameRate_ { 0 };
    std::deque<float>::const_iterator readIterator_;
    std::deque<float>::iterator writeIterator_;
};

}

#endif
