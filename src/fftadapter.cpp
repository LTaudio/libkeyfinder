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

#include "fftadapter.h"

// Included here to allow substitution of a separate implementation .cpp
#include <cmath>
#include <cstring>
#include <fftw3.h>

namespace KeyFinder {

std::mutex fftwPlanMutex;

class FftAdapterPrivate {
public:
    float* inputReal;
    fftwf_complex* outputComplex;
    fftwf_plan plan;
};

FftAdapter::FftAdapter(unsigned int inFrameSize)
    : priv(new FftAdapterPrivate)
{
    frameSize = inFrameSize;
    priv->inputReal = (float*)fftwf_malloc(sizeof(float) * frameSize);
    priv->outputComplex = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * frameSize);
    memset(priv->outputComplex, 0, sizeof(fftwf_complex) * frameSize);
    fftwPlanMutex.lock();
    priv->plan = fftwf_plan_dft_r2c_1d(frameSize, priv->inputReal, priv->outputComplex, FFTW_ESTIMATE);
    fftwPlanMutex.unlock();
}

FftAdapter::~FftAdapter()
{
    fftwf_destroy_plan(priv->plan);
    fftwf_free(priv->inputReal);
    fftwf_free(priv->outputComplex);
    delete priv;
}

auto FftAdapter::getFrameSize() const -> unsigned int
{
    return frameSize;
}

void FftAdapter::setInput(unsigned int i, float real)
{
    if (i >= frameSize) {
        std::ostringstream ss;
        ss << "Cannot set out-of-bounds sample (" << i << "/" << frameSize << ")";
        throw Exception(ss.str().c_str());
    }
    if (!std::isfinite(real)) {
        throw Exception("Cannot set sample to NaN");
    }
    priv->inputReal[i] = real;
}

auto FftAdapter::getOutputReal(unsigned int i) const -> float
{
    if (i >= frameSize) {
        std::ostringstream ss;
        ss << "Cannot get out-of-bounds sample (" << i << "/" << frameSize << ")";
        throw Exception(ss.str().c_str());
    }
    return priv->outputComplex[i][0];
}

auto FftAdapter::getOutputImaginary(unsigned int i) const -> float
{
    if (i >= frameSize) {
        std::ostringstream ss;
        ss << "Cannot get out-of-bounds sample (" << i << "/" << frameSize << ")";
        throw Exception(ss.str().c_str());
    }
    return priv->outputComplex[i][1];
}

auto FftAdapter::getOutputMagnitude(unsigned int i) const -> float
{
    if (i >= frameSize) {
        std::ostringstream ss;
        ss << "Cannot get out-of-bounds sample (" << i << "/" << frameSize << ")";
        throw Exception(ss.str().c_str());
    }
    return sqrt(pow(getOutputReal(i), 2) + pow(getOutputImaginary(i), 2));
}

void FftAdapter::execute()
{
    fftwf_execute(priv->plan);
}

// ================================= INVERSE =================================

class InverseFftAdapterPrivate {
public:
    fftwf_complex* inputComplex;
    float* outputReal;
    fftwf_plan plan;
};

InverseFftAdapter::InverseFftAdapter(unsigned int inFrameSize)
    : priv(new InverseFftAdapterPrivate)
{
    frameSize = inFrameSize;
    priv->inputComplex = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * frameSize);
    priv->outputReal = (float*)fftwf_malloc(sizeof(float) * frameSize);
    fftwPlanMutex.lock();
    priv->plan = fftwf_plan_dft_c2r_1d(frameSize, priv->inputComplex, priv->outputReal, FFTW_ESTIMATE);
    fftwPlanMutex.unlock();
}

InverseFftAdapter::~InverseFftAdapter()
{
    fftwf_destroy_plan(priv->plan);
    fftwf_free(priv->inputComplex);
    fftwf_free(priv->outputReal);
    delete priv;
}

auto InverseFftAdapter::getFrameSize() const -> unsigned int
{
    return frameSize;
}

void InverseFftAdapter::setInput(unsigned int i, float real, float imag)
{
    if (i >= frameSize) {
        std::ostringstream ss;
        ss << "Cannot set out-of-bounds sample (" << i << "/" << frameSize << ")";
        throw Exception(ss.str().c_str());
    }
    if (!std::isfinite(real) || !std::isfinite(imag)) {
        throw Exception("Cannot set sample to NaN");
    }
    priv->inputComplex[i][0] = real;
    priv->inputComplex[i][1] = imag;
}

auto InverseFftAdapter::getOutput(unsigned int i) const -> float
{
    if (i >= frameSize) {
        std::ostringstream ss;
        ss << "Cannot get out-of-bounds sample (" << i << "/" << frameSize << ")";
        throw Exception(ss.str().c_str());
    }
    // divide by frameSize to normalise
    return priv->outputReal[i] / frameSize;
}

void InverseFftAdapter::execute()
{
    fftwf_execute(priv->plan);
}

}
