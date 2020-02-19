#include "KitInstrument.h"
#include "MusicDeviceHolder.h"

KitInstrument::KitInstrument(MusicDeviceHolder &rMusicDeviceHolder) noexcept :
   m_rMusicDeviceHolder(rMusicDeviceHolder)
{
}

void KitInstrument::noteOn(int soundIndex, int note, float velocity) noexcept
{
   for(auto& voice : m_sounds.at(soundIndex).voices)
   {
      auto& rSoundDev = *m_rMusicDeviceHolder.soundDevices.atIdx(voice.soundDeviceIndex).second;
      rSoundDev.noteOn(voice.voiceIndex, note + voice.noteOffset, velocity);
   }
}

void KitInstrument::noteOff(int soundIndex, int note, float velocity) noexcept
{
   for(auto& voice : m_sounds.at(soundIndex).voices)
   {
      auto& rSoundDev = *m_rMusicDeviceHolder.soundDevices.atIdx(voice.soundDeviceIndex).second;
      rSoundDev.noteOff(voice.voiceIndex, note + voice.noteOffset, velocity);
   }
}