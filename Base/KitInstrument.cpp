#include "KitInstrument.h"
#include "MusicDeviceHolder.h"

void KitInstrument::init(MusicDeviceHolder &rMusicDeviceHolder) noexcept
{
   for(auto& sound : m_sounds)
   {
      sound.init(rMusicDeviceHolder);
   }
}

void KitInstrument::noteOn(int soundIndex, int note, float velocity) noexcept
{
   for(auto& voice : m_sounds.at(soundIndex).voices)
   {
      if(voice.pSoundDevice)
         voice.pSoundDevice->noteOn(voice.voiceIndex, note + voice.noteOffset, velocity);
   }
}

void KitInstrument::noteOff(int soundIndex, int note, float velocity) noexcept
{
   for(auto& voice : m_sounds.at(soundIndex).voices)
   {
      if(voice.pSoundDevice)
         voice.pSoundDevice->noteOff(voice.voiceIndex, note + voice.noteOffset, velocity);
   }
}

void KitInstrument::addKitSound(const KitSound& kitSound) noexcept
{
   m_sounds.emplace_back(kitSound);
}

void KitInstrument::addKitSound(KitSound&& kitSound) noexcept
{
   m_sounds.emplace_back(kitSound);
}