#include "MidiDumper.h"
#include "Midi.h"
#include "MusicDeviceHolder.h"

midi::Dumper::Dumper(MusicDeviceHolder& rMusicDeviceHolder) noexcept :
   m_data(rMusicDeviceHolder.controllerDevices.size())
{
   for(auto cdIdx = 0; cdIdx < rMusicDeviceHolder.controllerDevices.size(); ++cdIdx)
   {
      auto &controllerDev = rMusicDeviceHolder.controllerDevices.atIdx(cdIdx);
      auto &midiInHandler = controllerDev.second->m_midiInMsgHandler;
      if(!midiInHandler) continue;
      midiInHandler->m_midiIn.registerMidiInCb([this, cdIdx](const midi::MidiMessage& midiMsg){
         if(mpark::holds_alternative<midi::Message<midi::Clock>>(midiMsg))
         {
            m_data[cdIdx].clockReceived = true;
         }
         else
         {
            {
               std::unique_lock lock(m_data[cdIdx].ringBufLock);
               m_data[cdIdx].ringBuf.push(midiMsg);
            }
            if(m_ringBufChangedCb) m_ringBufChangedCb(cdIdx);
         }
      });
   }
}

midi::Dumper::RingBuf midi::Dumper::getRingBuf(int devIdx) const noexcept
{
   if(devIdx < 0 || devIdx >= m_data.size()) return RingBuf();
   std::unique_lock lock(m_data[devIdx].ringBufLock);
   return m_data[devIdx].ringBuf;
}

bool midi::Dumper::getClockReceived(int devIdx) const noexcept
{
   if(devIdx < 0 || devIdx >= m_data.size()) return false;
   return m_data[devIdx].clockReceived;
}

void midi::Dumper::resetClockReceived(int devIdx) noexcept
{
   if(devIdx < 0 || devIdx >= m_data.size()) return;
   m_data[devIdx].clockReceived = false;
}

void midi::Dumper::registerRingbufChangedCb(std::function<void(int)> cb) noexcept
{
   m_ringBufChangedCb = cb;
}