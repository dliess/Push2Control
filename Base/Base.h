#ifndef BASE_H
#define BASE_H

#include <memory>
#include <vector>

#include "Instruments.h"
#include "InstrumentsFactory.h"
#include "KitInstrument.h"
#include "MusicDeviceFactory.h"
#include "MusicDeviceHolder.h"
#include "TempoHandler.h"
#include "ThreadedLoop.h"

namespace base
{
struct Base
{
public:
   Base();
   MusicDeviceHolder musicDeviceHolder;
   MusicDeviceFactory musicDeviceFactory;
   Instruments instruments;
   InstrumentsFactory instrumentsFactory;
   TempoHandler tempoHandler;

   void start();
   void waitForEnd();

private:
   std::unique_ptr<util::ThreadedLoop> m_pMidiOutThreadLoop;
   void threadFunction();
};

} // namespace base

#endif