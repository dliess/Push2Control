#ifndef BASE_H
#define BASE_H

#include <vector>
#include <memory>

#include "Instruments.h"
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
   Instruments instruments;
   MusicDeviceHolder musicDeviceHolder;
   MusicDeviceFactory musicDeviceFactory;
   TempoHandler tempoHandler;

   void start();
   void waitForEnd();

private:
   std::unique_ptr<util::ThreadedLoop> m_pMidiOutThreadLoop;
   void threadFunction();
};

} // namespace base

#endif