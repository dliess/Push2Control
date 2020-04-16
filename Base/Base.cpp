#include "Base.h"

#include <cassert>
#include <exception>
#include <loguru.hpp>

#include "ThreadedLoop.h"
#include "UsbMidiPortNotifier.h"

base::Base::Base(const std::string& configDir) :
   musicDeviceHolder(),
   musicDeviceFactory(musicDeviceHolder, configDir),
   instruments(),
   instrumentsFactory(instruments, musicDeviceHolder),
   tempoHandler(musicDeviceHolder.musicDevices)
{
   // TODO: Remove Dummy
   instruments.load("relDir", "filename", "section");
}

void base::Base::start()
{
   if (!midi::PortNotifiers::instance().init())
   {
      // TODO: put this code to Midi lib
      throw std::runtime_error("midi::PortNotifiers::instance().init() failed");
   }
   m_pMidiOutThreadLoop = std::make_unique<util::ThreadedLoop>(
      std::chrono::milliseconds(500), [this]() { threadFunction(); });
}

void base::Base::waitForEnd()
{
   assert(m_pMidiOutThreadLoop);
   m_pMidiOutThreadLoop->join();
}

void base::Base::threadFunction()
{
   // LOG_SCOPE_FUNCTION(INFO);

   // VLOG_SCOPE_F(0, "Base::threadFunction()");
   {
      // VLOG_SCOPE_F(1, "PortNotifier update");
      midi::PortNotifiers::instance().update();
   }
   musicDeviceHolder.processMidiInBuffers();
   tempoHandler.nextTimeSlot();
   musicDeviceHolder.processMidiOutBuffers();
}
