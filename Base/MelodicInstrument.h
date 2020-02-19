#ifndef MELODY_TRACK_H
#define MELODY_TRACK_H

#include <vector>
#include <array>
#include "MusicDeviceHolder.h"

class MusicDevice;

class MelodicInstrument
{
public:
   MelodicInstrument(MusicDeviceHolder &rMusicDeviceHolder) noexcept;
   void noteOn(int note, float velocity) noexcept;
   void noteOff(int note, float velocity) noexcept;
   void pitchBend(float value) noexcept;
   void parameterChange(int parameterId, float value) noexcept;

   void noteOn(int voiceIdx, int note, float velocity) noexcept;
   void noteOff(int voiceIdx, int note, float velocity) noexcept;
   void pitchBend(int voiceIdx, float value) noexcept;
   void parameterChange(int voiceIdx, int parameterId, float value) noexcept;

private:
   MusicDeviceHolder &m_rMusicDeviceHolder;
   struct VoiceId
   {
      int soundDeviceIndex;
      int voiceIndex;
   };
   using VoiceContainer = std::vector<VoiceId>;
   VoiceContainer           m_voices;
   int                      m_currentVoiceIndex{-1};

   static constexpr int NUM_NOTES = 128;
   static constexpr int FREE = -1;
   std::array<int, NUM_NOTES> m_noteAllocations{FREE};

   inline void incrementVoiceIndex() noexcept
   {
      if(m_voices.empty()) return;
      ++m_currentVoiceIndex;
      if(m_currentVoiceIndex == m_voices.size())
      {
         m_currentVoiceIndex = 0;
      }
   }
   inline MusicDevice& soundDevice(int soundDeviceIdx) const
   {
      return *m_rMusicDeviceHolder.soundDevices.atIdx(soundDeviceIdx).second;
   }
};

#endif // MELODY_TRACK_H