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

#include "audiodata.h"

namespace KeyFinder {

AudioData::AudioData()
    : samples_(0)

{
}

auto AudioData::getChannels() const -> unsigned int
{
    return channels_;
}

void AudioData::setChannels(unsigned int inChannels)
{
    if (inChannels < 1) {
        throw Exception("New channel count must be > 0");
    }
    channels_ = inChannels;
}

auto AudioData::getFrameRate() const -> unsigned int
{
    return frameRate_;
}

void AudioData::setFrameRate(unsigned int inFrameRate)
{
    if (inFrameRate < 1) {
        throw Exception("New frame rate must be > 0");
    }
    frameRate_ = inFrameRate;
}

void AudioData::append(const AudioData& that)
{
    if (channels_ == 0 && frameRate_ == 0) {
        channels_ = that.channels_;
        frameRate_ = that.frameRate_;
    }
    if (that.channels_ != channels_) {
        throw Exception("Cannot append audio data with a different number of channels");
    }
    if (that.frameRate_ != frameRate_) {
        throw Exception("Cannot append audio data with a different frame rate");
    }
    samples_.insert(samples_.end(), that.samples_.begin(), that.samples_.end());
}

void AudioData::prepend(const AudioData& that)
{
    if (channels_ == 0 && frameRate_ == 0) {
        channels_ = that.channels_;
        frameRate_ = that.frameRate_;
    }
    if (that.channels_ != channels_) {
        throw Exception("Cannot prepend audio data with a different number of channels");
    }
    if (that.frameRate_ != frameRate_) {
        throw Exception("Cannot prepend audio data with a different frame rate");
    }
    samples_.insert(samples_.begin(), that.samples_.begin(), that.samples_.end());
}

// get sample by absolute index
auto AudioData::getSample(unsigned int index) const -> double
{
    if (index >= getSampleCount()) {
        std::ostringstream ss;
        ss << "Cannot get out-of-bounds sample (" << index << "/" << getSampleCount() << ")";
        throw Exception(ss.str().c_str());
    }
    return samples_[index];
}

// get sample by frame and channel
auto AudioData::getSampleByFrame(unsigned int frame, unsigned int channel) const -> double
{
    if (frame >= getFrameCount()) {
        std::ostringstream ss;
        ss << "Cannot get out-of-bounds frame (" << frame << "/" << getFrameCount() << ")";
        throw Exception(ss.str().c_str());
    }
    if (channel >= channels_) {
        std::ostringstream ss;
        ss << "Cannot get out-of-bounds channel (" << channel << "/" << channels_ << ")";
        throw Exception(ss.str().c_str());
    }
    return getSample(frame * channels_ + channel);
}

// set sample by absolute index
void AudioData::setSample(unsigned int index, double value)
{
    if (index >= getSampleCount()) {
        std::ostringstream ss;
        ss << "Cannot set out-of-bounds sample (" << index << "/" << getSampleCount() << ")";
        throw Exception(ss.str().c_str());
    }
    if (!std::isfinite(value)) {
        throw Exception("Cannot set sample to NaN");
    }
    samples_[index] = value;
}

// set sample by frame and channel
void AudioData::setSampleByFrame(unsigned int frame, unsigned int channel, double value)
{
    if (frame >= getFrameCount()) {
        std::ostringstream ss;
        ss << "Cannot set out-of-bounds frame (" << frame << "/" << getFrameCount() << ")";
        throw Exception(ss.str().c_str());
    }
    if (channel >= channels_) {
        std::ostringstream ss;
        ss << "Cannot set out-of-bounds channel (" << channel << "/" << channels_ << ")";
        throw Exception(ss.str().c_str());
    }
    setSample(frame * channels_ + channel, value);
}

void AudioData::addToSampleCount(unsigned int inSamples)
{
    samples_.resize(getSampleCount() + inSamples, 0.0);
}

void AudioData::addToFrameCount(unsigned int inFrames)
{
    if (channels_ < 1) {
        throw Exception("Channels must be > 0");
    }
    addToSampleCount(inFrames * channels_);
}

auto AudioData::getSampleCount() const -> unsigned int
{
    return samples_.size();
}

auto AudioData::getFrameCount() const -> unsigned int
{
    if (channels_ < 1) {
        throw Exception("Channels must be > 0");
    }
    return getSampleCount() / channels_;
}

void AudioData::reduceToMono()
{
    if (channels_ < 2) {
        return;
    }
    auto readAt = samples_.begin();
    auto writeAt = samples_.begin();
    while (readAt < samples_.end()) {
        double sum = 0.0;
        for (unsigned int c = 0; c < channels_; c++) {
            sum += *readAt;
            std::advance(readAt, 1);
        }
        *writeAt = sum / channels_;
        std::advance(writeAt, 1);
    }
    samples_.resize(getSampleCount() / channels_);
    channels_ = 1;
}

// Strictly to be applied AFTER low pass filtering
void AudioData::downsample(unsigned int factor, bool shortcut)
{
    if (factor == 1) {
        return;
    }
    if (channels_ > 1) {
        throw Exception("Apply to monophonic only");
    }
    auto readAt = samples_.begin();
    auto writeAt = samples_.begin();

    // Prevent std::advance out of iterator range problems
    size_t numSamplesRemaining = samples_.size();

    while (readAt < samples_.end()) {
        double mean = 0.0;
        if (shortcut) {
            mean = *readAt;
            if (numSamplesRemaining >= factor) {
                std::advance(readAt, factor);
            } else {
                readAt = samples_.end();
            }
            numSamplesRemaining -= factor;
        } else {
            for (unsigned int s = 0; s < factor; s++) {
                if (readAt < samples_.end()) {
                    mean += *readAt;
                    std::advance(readAt, 1);
                    --numSamplesRemaining;
                }
                mean /= (double)factor;
            }
        }
        *writeAt = mean;
        std::advance(writeAt, 1);
    }
    samples_.resize(ceil((double)getSampleCount() / (double)factor));
    setFrameRate(getFrameRate() / factor);
}

void AudioData::discardFramesFromFront(unsigned int discardFrameCount)
{
    if (discardFrameCount > getFrameCount()) {
        std::ostringstream ss;
        ss << "Cannot discard " << discardFrameCount << " frames of " << getFrameCount();
        throw Exception(ss.str().c_str());
    }
    unsigned int discardSampleCount = discardFrameCount * channels_;
    auto discardToHere = samples_.begin();
    std::advance(discardToHere, discardSampleCount);
    samples_.erase(samples_.begin(), discardToHere);
}

auto AudioData::sliceSamplesFromBack(unsigned int sliceSampleCount) -> AudioData*
{

    if (sliceSampleCount > getSampleCount()) {
        std::ostringstream ss;
        ss << "Cannot slice " << sliceSampleCount << " samples of " << getSampleCount();
        throw Exception(ss.str().c_str());
    }

    unsigned int samplesToLeaveIntact = getSampleCount() - sliceSampleCount;

    auto* that = new AudioData();
    that->channels_ = channels_;
    that->setFrameRate(getFrameRate());
    that->addToSampleCount(sliceSampleCount);

    auto readAt = samples_.begin();
    std::advance(readAt, samplesToLeaveIntact);
    auto writeAt = that->samples_.begin();
    while (readAt < samples_.end()) {
        *writeAt = *readAt;
        std::advance(readAt, 1);
        std::advance(writeAt, 1);
    }

    samples_.resize(samplesToLeaveIntact);

    return that;
}

void AudioData::resetIterators()
{
    readIterator_ = samples_.begin();
    writeIterator_ = samples_.begin();
}

auto AudioData::readIteratorWithinUpperBound() const -> bool
{
    return (readIterator_ < samples_.end());
}

auto AudioData::writeIteratorWithinUpperBound() const -> bool
{
    return (writeIterator_ < samples_.end());
}

void AudioData::advanceReadIterator(unsigned int by)
{
    std::advance(readIterator_, by);
}

void AudioData::advanceWriteIterator(unsigned int by)
{
    std::advance(writeIterator_, by);
}

auto AudioData::getSampleAtReadIterator() const -> double
{
    return *readIterator_;
}

void AudioData::setSampleAtWriteIterator(double value)
{
    *writeIterator_ = value;
}

}
