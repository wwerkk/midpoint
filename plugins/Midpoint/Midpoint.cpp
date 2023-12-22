// PluginMidpoint.cpp
// W K Werkowicz (wwerkowicz@gmail.com)

#include "SC_PlugIn.hpp"
#include "Midpoint.hpp"

static InterfaceTable* ft;

namespace Midpoint {

Midpoint::Midpoint() {
    mCalcFunc = make_calc_function<Midpoint, &Midpoint::next>();
    // initialize a constant for multiplying the frequency
    mFreqMul = sampleDur();
    // get depth param
    mDepth = in0(1);
    // get spread param
    mSpread = in0(2);
    // get reduction param
    mReduction = in0(3);
    // allocate and fill the buffer
    buf = (float*) RTAlloc(mWorld, 8192 * sizeof(float));

    if (buf == NULL) {
        mCalcFunc = make_calc_function<Midpoint, &Midpoint::clear>();
        clear(1);
        if(mWorld->mVerbosity > -2) {
            Print("Failed to allocate memory for Midpoint ugen.\n");
        }
        return;
    }
    memset(buf, 0, 8192 * sizeof(float));
    // Initialize first and last elements
    buf[0] = 0;
    buf[1] = 1;
    buf[2] = 0;
    buf[3] = -1;
    buf[4] = 0;
    // init size
    mSize = 5;
    // run midpoint displacement
    for (int i = 0; i < mDepth; ++i) {
        subdiv(buf, mSize, mSpread * powf(mReduction, i));
    }
    
    // start with phase at 0
    mPhase = 0.0;

    // output a single sample
    next(1);
}

Midpoint::~Midpoint() {
    // Free the memory.
    RTFree(mWorld, buf);
}

void Midpoint::clear(int inNumSamples) {
        ClearUnitOutputs(this, inNumSamples);
}

void Midpoint::next(int nSamples) {
  // get the pointer to the output buffer
    float *outBuf = out(0);
    const float *freq = in(0);
    const float *depth = in(1);
    const float *spread = in(2);
    const float *reduction = in(3);

    double freqmul = mFreqMul;
    float phase = mPhase;

    for (int i=0; i < nSamples; ++i) {
        if (phase >= 1.f) {
            phase -= 1.f;
            mDepth = depth[(inRate(1) != calc_ScalarRate) * i];
            mSpread = spread[(inRate(2) != calc_ScalarRate) * i];
            mReduction = reduction[(inRate(3) != calc_ScalarRate) * i];
            buf[0] = 0;
            buf[1] = 1;
            buf[2] = 0;
            buf[3] = -1;
            buf[4] = 0;
            mSize = 5;

            for (int i = 0; i < mDepth; ++i) {
                subdiv(buf, mSize, mSpread * powf(mReduction, i));
            }

        }
        int ix = int(phase * mSize);
        float z = buf[ix];
        float freq_ = freq[(inRate(0) != calc_ScalarRate) * i];
        
        phase += freq_ * mFreqMul;
        outBuf[i] = z;
    }


    mPhase = phase;

}

void Midpoint::subdiv(float* b, int size, float spread) {
    if (size * 2 - 1 > 8192) return;
    float* tmp = (float*) RTAlloc(mWorld, (8192) * sizeof(float));
    memset(tmp, 0, 8192 * sizeof(float));

    // Calculate midpoints with spread and store them in tmp
    for (int i = 0; i < size - 1; ++i) {
        float mid = (b[i] + b[(i + 1) % size]) * 0.5f;
        mid += rgen.frand2() * spread;
        mid = sc_fold1(mid);
        tmp[i] = mid;
    }

    // Make space in b
    for (int i = size - 1; i > 0; --i) {
        b[2 * i] = b[i];
    }

    // Interleave tmp into b
    for (int i = 0; i < size - 1; ++i) {
        b[2 * i + 1] = tmp[i];
    }

    // Free tmp
    RTFree(mWorld, tmp);

    // Update mSize
    mSize -= 1;
    mSize = size * 2 - 1;

}

} // namespace Midpoint

PluginLoad(MidpointUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<Midpoint::Midpoint>(ft, "Midpoint", false);
}
