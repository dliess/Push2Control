#ifndef BASE_H
#define BASE_H

#include "KitInstrument.h"
#include "MelodicInstrument.h"
#include <vector>

namespace base
{

struct Base
{
    struct Instruments
    {
        std::vector<KitInstrument>     kitInstruments;
        std::vector<MelodicInstrument> melodicInstruments;
    };
    Instruments instruments;
};

} // namespace base

#endif