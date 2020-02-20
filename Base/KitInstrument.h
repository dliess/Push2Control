#ifndef KIT_TRACK_H
#define KIT_TRACK_H

#include <vector>
#include "FixedSizeVector.h"

struct MusicDeviceHolder;

class KitInstrument
{
public:
   KitInstrument(MusicDeviceHolder &rMusicDeviceHolder) noexcept;
   void noteOn(int soundIndex, int note, float velocity) noexcept;
   void noteOff(int soundIndex, int note, float velocity) noexcept;
private:
   struct VoiceDescr
   {
      int soundDeviceIndex;
      int voiceIndex;
      int noteOffset;
   };
   struct KitSound
   {
      static constexpr int NUM_MAX_VOICES_PER_KIT_VOICE = 4;
      using Voices = util::FixedSizeVector<VoiceDescr, NUM_MAX_VOICES_PER_KIT_VOICE>;
      Voices voices;
   };
   MusicDeviceHolder    &m_rMusicDeviceHolder;
   std::vector<KitSound> m_sounds;
};

#endif // KIT_TRACK_H