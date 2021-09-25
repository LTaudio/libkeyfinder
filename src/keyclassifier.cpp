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

#include "keyclassifier.h"

#include <math.h>

namespace KeyFinder {

KeyClassifier::KeyClassifier(const std::vector<float>& majorProfile, const std::vector<float>& minorProfile)
{

    if (majorProfile.size() != BANDS) {
        throw Exception("Tone profile must have 72 elements");
    }

    if (minorProfile.size() != BANDS) {
        throw Exception("Tone profile must have 72 elements");
    }

    major_ = new ToneProfile(majorProfile);
    minor_ = new ToneProfile(minorProfile);
    silence_ = new ToneProfile(std::vector<float>(BANDS, 0.0));
}

KeyClassifier::~KeyClassifier()
{
    delete major_;
    delete minor_;
    delete silence_;
}

auto KeyClassifier::classify(const std::vector<float>& chromaVector) -> KeyT
{
    std::vector<float> scores(24);
    float bestScore = 0.0;
    for (unsigned int i = 0; i < SEMITONES; i++) {
        float score = NAN;
        score = major_->cosineSimilarity(chromaVector, i); // major
        scores[i * 2] = score;
        score = minor_->cosineSimilarity(chromaVector, i); // minor
        scores[(i * 2) + 1] = score;
    }
    bestScore = silence_->cosineSimilarity(chromaVector, 0);
    // find best match, defaulting to silence
    KeyT bestMatch = SILENCE;
    for (unsigned int i = 0; i < 24; i++) {
        if (scores[i] > bestScore) {
            bestScore = scores[i];
            bestMatch = (KeyT)i;
        }
    }
    return bestMatch;
}

}
