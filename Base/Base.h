#ifndef BASE_H
#define BASE_H

#include "KitInstrument.h"
#include "MelodicInstrument.h"
#include "MusicDeviceHolder.h"
#include "MusicDeviceFactory.h"
#include <vector>

namespace base
{
struct Instruments
{
    std::vector<KitInstrument>     kitInstruments;
    std::vector<MelodicInstrument> melodicInstruments;
};

struct Base
{
    Base() noexcept :
        instruments(),
        musicDeviceHolder(),
        musicDeviceFactory(musicDeviceHolder, instruments)
    {}
    Instruments        instruments;
    MusicDeviceHolder  musicDeviceHolder;
    MusicDeviceFactory musicDeviceFactory;
};

} // namespace base

#endif