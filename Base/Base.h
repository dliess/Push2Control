#ifndef BASE_H
#define BASE_H

#include "KitInstrument.h"
#include "MusicDeviceHolder.h"
#include "MusicDeviceFactory.h"
#include "Instruments.h"
#include <vector>

namespace base
{

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