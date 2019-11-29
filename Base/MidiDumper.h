#ifndef MIDI_DUMPER_H
#define MIDI_DUMPER_H

#include <vector>
#include <functional>
#include <atomic>
#include <mutex>
#include "Ringbuffer.hpp"
#include "MidiMessage.h"

class MusicDeviceHolder;

namespace midi
{

class Dumper
{
public:
   Dumper(MusicDeviceHolder& rMusicDeviceHolder) noexcept;
   using RingBuf = util::Ringbuffer<MidiMessage, 64>;
   RingBuf getRingBuf(int devIdx) const noexcept;
   bool getClockReceived(int devIdx) const noexcept;
   void resetClockReceived(int devIdx) noexcept;
   void registerRingbufChangedCb(std::function<void(int)> cb) noexcept;

private:
   struct Data
   {
      RingBuf                   ringBuf;
      std::atomic<bool>         clockReceived{false};
      mutable std::mutex        ringBufLock;
   };
   std::vector<Data>        m_data;
   std::function<void(int)> m_ringBufChangedCb;
};

} // namespace midi

#endif