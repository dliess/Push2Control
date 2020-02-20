#ifndef BASE_INSTRUMENTS_H
#define BASE_INSTRUMENTS_H

#include "KitInstrument.h"
#include "MelodicInstrument.h"
#include <vector>

namespace base
{

struct Instruments
{
    std::vector<KitInstrument>     kitInstruments;
    std::vector<MelodicInstrument> melodicInstruments;
};

} // namespace base

#endif