// PluginMidpoint.hpp
// W K Werkowicz (wwerkowicz@gmail.com)

#pragma once

#include "SC_PlugIn.hpp"
#include "SC_RGen.h"

namespace Midpoint {

class Midpoint : public SCUnit {
public:
    Midpoint();

    // Destructor
    ~Midpoint();

private:

    // Clear function
    void clear(int inNumSamples);
    
    // Calc function
    void next(int nSamples);

    void subdiv(float* b, float* b_, int size, float spread);
    // void rSubdiv(World *mWorld, const float* input, float* output, int inputSize, int depth);

    // Member variables
    RGen & rgen = *mParent -> mRGen;


    float mPhase; // phase of the oscillator, from 0 to 1.
    double mFreqMul; // a constant for multiplying frequency

    int mSize;
    int mSize_;
    float mDepth;
    float mSpread;
    float mReduction;
    float* buf;
    float* buf_; // copy to keep past version of the buffer
};

} // namespace Midpoint
