// PluginMidpoint.cpp
// W K Werkowicz (wwerkowicz@gmail.com)

#include "SC_PlugIn.hpp"
#include "Midpoint.hpp"

static InterfaceTable* ft;

namespace Midpoint {

Midpoint::Midpoint() {
    mCalcFunc = make_calc_function<Midpoint, &Midpoint::next>();
    next(1);
}

void Midpoint::next(int nSamples) {

    // Audio rate input
    const float* input = in(0);

    // Control rate parameter: gain.
    const float gain = in0(1);

    // Output buffer
    float* outbuf = out(0);

    // simple gain function
    for (int i = 0; i < nSamples; ++i) {
        outbuf[i] = input[i] * gain;
    }
}

} // namespace Midpoint

PluginLoad(MidpointUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<Midpoint::Midpoint>(ft, "Midpoint", false);
}
