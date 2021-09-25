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

#include "chromatransformfactory.h"

namespace KeyFinder {

ChromaTransformFactory::ChromaTransformWrapper::ChromaTransformWrapper(unsigned int inFrameRate, const ChromaTransform* const inChromaTransform)
    : frameRate_(inFrameRate)
    , chromaTransform_(inChromaTransform)
{
}

ChromaTransformFactory::ChromaTransformWrapper::~ChromaTransformWrapper()
{
    delete chromaTransform_;
}

auto ChromaTransformFactory::ChromaTransformWrapper::getChromaTransform() const -> const ChromaTransform*
{
    return chromaTransform_;
}

auto ChromaTransformFactory::ChromaTransformWrapper::getFrameRate() const -> unsigned int
{
    return frameRate_;
}

ChromaTransformFactory::ChromaTransformFactory()
    : chromaTransforms_(0)
{
}

ChromaTransformFactory::~ChromaTransformFactory()
{
    for (auto& chromaTransform : chromaTransforms_) {
        delete chromaTransform;
    }
}

auto ChromaTransformFactory::getChromaTransform(unsigned int frameRate) -> const ChromaTransform*
{
    for (auto* wrapper : chromaTransforms_) {
        if (wrapper->getFrameRate() == frameRate) {
            return wrapper->getChromaTransform();
        }
    }
    chromaTransformFactoryMutex_.lock();
    chromaTransforms_.push_back(new ChromaTransformWrapper(frameRate, new ChromaTransform(frameRate)));
    unsigned int newChromaTransformIndex = chromaTransforms_.size() - 1;
    chromaTransformFactoryMutex_.unlock();
    return chromaTransforms_[newChromaTransformIndex]->getChromaTransform();
}

}
