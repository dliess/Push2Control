#ifndef BASE_H
#define BASE_H

#include "KitInstrument.h"
#include "MusicDeviceHolder.h"
#include "MusicDeviceFactory.h"
#include "Instruments.h"
#include "TempoHandler.h"
#include "ThreadedLoop.h"
#include <vector>

namespace base
{

struct Base
{
    Base() noexcept;
    Instruments        instruments;
    MusicDeviceHolder  musicDeviceHolder;
    MusicDeviceFactory musicDeviceFactory;
    TempoHandler       tempoHandler;

private:
    util::ThreadedLoop m_midiOutThreadLoop;
    void threadLoop();
    void threadFunction();
};

} // namespace base

#endif