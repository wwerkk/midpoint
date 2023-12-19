// PluginMidpoint.hpp
// W K Werkowicz (wwerkowicz@gmail.com)

#pragma once

#include "SC_PlugIn.hpp"

namespace Midpoint {

class Midpoint : public SCUnit {
public:
    Midpoint();

    // Destructor
    // ~Midpoint();

private:
    // Calc function
    void next(int nSamples);

    // Member variables
};

} // namespace Midpoint
