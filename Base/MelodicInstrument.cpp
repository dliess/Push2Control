#include "MelodicInstrument.h"

void MelodicInstrument::init(MusicDeviceHolder &rMusicDeviceHolder) noexcept
{
   for(auto& voice : m_voices)
   {
      auto iter = rMusicDeviceHolder.soundDevices.find(voice.soundDeviceId);
      if(rMusicDeviceHolder.soundDevices.end() != iter)
      {
         voice.pSoundDevice = iter->second;
      }
   }
}

void MelodicInstrument::noteOn(int note, float velocity) noexcept
{
   if(m_noteAllocations[note] != FREE) return;
   incrementVoiceIndex();
   m_noteAllocations[note] = m_currentVoiceIndex;
   if(m_voices[m_currentVoiceIndex].pSoundDevice)
      m_voices[m_currentVoiceIndex].pSoundDevice->noteOn(m_voices[m_currentVoiceIndex].voiceIndex, note, velocity);
}

void MelodicInstrument::noteOff(int note, float velocity) noexcept
{
   if(m_noteAllocations[note] == FREE) return;
   const auto& voice = m_voices[m_noteAllocations[note]];
   voice.pSoundDevice->noteOff(voice.voiceIndex, note, velocity);
   m_noteAllocations[note] = FREE;
}

void MelodicInstrument::noteOn(int voiceIdx, int note, float velocity) noexcept
{
   const auto voiceIndex = voiceIdx % m_voices.size();
   if(m_voices[m_currentVoiceIndex].pSoundDevice)
      m_voices[m_currentVoiceIndex].pSoundDevice->noteOn(m_voices[voiceIndex].voiceIndex, note, velocity);
}

void MelodicInstrument::noteOff(int voiceIdx, int note, float velocity) noexcept
{
   const auto voiceIndex = voiceIdx % m_voices.size();
   if(m_voices[m_currentVoiceIndex].pSoundDevice)
      m_voices[m_currentVoiceIndex].pSoundDevice->noteOff(m_voices[voiceIndex].voiceIndex, note, velocity);
}

void MelodicInstrument::pitchBend(float value) noexcept
{
   for(auto& voiceDescr : m_voices)
   {
      if(m_voices[m_currentVoiceIndex].pSoundDevice)
         m_voices[m_currentVoiceIndex].pSoundDevice->pitchBend(voiceDescr.voiceIndex, value);
   }
}

void MelodicInstrument::pitchBend(int voiceIdx, float value) noexcept
{
   const auto voiceIndex = voiceIdx % m_voices.size();
   if(m_voices[m_currentVoiceIndex].pSoundDevice)
      m_voices[m_currentVoiceIndex].pSoundDevice->pitchBend(m_voices[voiceIndex].voiceIndex, value);
}

void MelodicInstrument::parameterChange(int parameterId, float value) noexcept
{
   for(auto& voiceDescr : m_voices)
   {
      if(m_voices[m_currentVoiceIndex].pSoundDevice)
         m_voices[m_currentVoiceIndex].pSoundDevice->setSoundParameterValue(voiceDescr.voiceIndex, parameterId, value);
   }
}

void MelodicInstrument::parameterChange(int voiceIdx, int parameterId, float value) noexcept
{
   const auto voiceIndex = voiceIdx % m_voices.size();
   if(m_voices[m_currentVoiceIndex].pSoundDevice)
      m_voices[m_currentVoiceIndex].pSoundDevice->setSoundParameterValue(m_voices[voiceIndex].voiceIndex, parameterId, value);
}