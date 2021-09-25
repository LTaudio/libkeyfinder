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

#ifndef KEYFINDER_H
#define KEYFINDER_H

#include "audiodata.h"
#include "chromatransformfactory.h"
#include "keyclassifier.h"
#include "lowpassfilterfactory.h"
#include "spectrumanalyser.h"

namespace KeyFinder {

class KeyFinder {
public:
    // for progressive analysis
    void progressiveChromagram(AudioData audio, Workspace& workspace);
    void finalChromagram(Workspace& workspace);
    [[nodiscard]] static auto keyOfChromagram(const Workspace& workspace) -> KeyT;

    // for analysis of a whole audio file
    auto keyOfAudio(const AudioData& audio) -> KeyT;

    // for experimentation with alternative tone profiles
    [[nodiscard]] static auto keyOfChromaVector(const std::vector<double>& chromaVector, const std::vector<double>& overrideMajorProfile, const std::vector<double>& overrideMinorProfile) -> KeyT;

private:
    void preprocess(AudioData& workingAudio, Workspace& workspace, bool flushRemainderBuffer = false);
    void chromagramOfBufferedAudio(Workspace& workspace);
    [[nodiscard]] static auto keyOfChromaVector(const std::vector<double>& chromaVector) -> KeyT;
    LowPassFilterFactory lpfFactory_;
    ChromaTransformFactory ctFactory_;
    TemporalWindowFactory twFactory_;
};

}

#endif
