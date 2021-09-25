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

#ifndef FFTADAPTER_H
#define FFTADAPTER_H

#include "constants.h"

namespace KeyFinder {

class FftAdapterPrivate;
class InverseFftAdapterPrivate;

class FftAdapter {
public:
    FftAdapter(unsigned int frameSize);
    ~FftAdapter();
    [[nodiscard]] auto getFrameSize() const -> unsigned int;
    void setInput(unsigned int i, double real);
    void execute();
    [[nodiscard]] auto getOutputReal(unsigned int i) const -> double;
    [[nodiscard]] auto getOutputImaginary(unsigned int i) const -> double;
    [[nodiscard]] auto getOutputMagnitude(unsigned int i) const -> double;

protected:
    unsigned int frameSize;
    FftAdapterPrivate* priv;
};

class InverseFftAdapter {
public:
    InverseFftAdapter(unsigned int frameSize);
    ~InverseFftAdapter();
    [[nodiscard]] auto getFrameSize() const -> unsigned int;
    void setInput(unsigned int i, double real, double imaginary);
    void execute();
    [[nodiscard]] auto getOutput(unsigned int i) const -> double;

protected:
    unsigned int frameSize;
    InverseFftAdapterPrivate* priv;
};

}

#endif
