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

#ifndef CHROMATRANSFORMFACTORY_H
#define CHROMATRANSFORMFACTORY_H

#include "chromatransform.h"
#include "constants.h"

namespace KeyFinder {

class ChromaTransformFactory {
public:
    ChromaTransformFactory();
    ~ChromaTransformFactory();
    auto getChromaTransform(unsigned int frameRate) -> const ChromaTransform*;

private:
    class ChromaTransformWrapper;
    std::vector<ChromaTransformWrapper*> chromaTransforms_;
    std::mutex chromaTransformFactoryMutex_;
};

class ChromaTransformFactory::ChromaTransformWrapper {
public:
    ChromaTransformWrapper(unsigned int frameRate, const ChromaTransform* transform);
    ~ChromaTransformWrapper();
    [[nodiscard]] auto getChromaTransform() const -> const ChromaTransform*;
    [[nodiscard]] auto getFrameRate() const -> unsigned int;

private:
    unsigned int frameRate_;
    const ChromaTransform* const chromaTransform_;
};

}

#endif
