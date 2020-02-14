#include "KitTrack.h"
#include "MusicDeviceHolder.h"

KitTrack::KitTrack(MusicDeviceHolder &rMusicDeviceHolder) noexcept :
   m_rMusicDeviceHolder(rMusicDeviceHolder)
{
}

void KitTrack::noteOn(int soundIndex, int note, float velocity) noexcept
{
   for(auto& voice : m_sounds.at(soundIndex).voices)
   {
      auto& rSoundDev = *m_rMusicDeviceHolder.soundDevices.atIdx(voice.soundDeviceIndex).second;
      rSoundDev.noteOn(voice.voiceIndex, note + voice.noteOffset, velocity);
   }
}

void KitTrack::noteOff(int soundIndex, int note, float velocity) noexcept
{
   for(auto& voice : m_sounds.at(soundIndex).voices)
   {
      auto& rSoundDev = *m_rMusicDeviceHolder.soundDevices.atIdx(voice.soundDeviceIndex).second;
      rSoundDev.noteOff(voice.voiceIndex, note + voice.noteOffset, velocity);
   }
}