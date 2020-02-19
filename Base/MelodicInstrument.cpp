#include "MelodicInstrument.h"

MelodicInstrument::MelodicInstrument(MusicDeviceHolder &rMusicDeviceHolder) noexcept :
   m_rMusicDeviceHolder(rMusicDeviceHolder)
{
}

void MelodicInstrument::noteOn(int note, float velocity) noexcept
{
   if(m_noteAllocations[note] != FREE) return;
   incrementVoiceIndex();
   m_noteAllocations[note] = m_currentVoiceIndex;
   soundDevice(m_voices[m_currentVoiceIndex].soundDeviceIndex).noteOn(m_voices[m_currentVoiceIndex].voiceIndex, note, velocity);
}

void MelodicInstrument::noteOff(int note, float velocity) noexcept
{
   if(m_noteAllocations[note] == FREE) return;
   const auto& voice = m_voices[m_noteAllocations[note]];
   soundDevice(voice.soundDeviceIndex).noteOff(voice.voiceIndex, note, velocity);
   m_noteAllocations[note] = FREE;
}

void MelodicInstrument::noteOn(int voiceIdx, int note, float velocity) noexcept
{
   const auto voiceIndex = voiceIdx % m_voices.size();
   soundDevice(m_voices[voiceIndex].soundDeviceIndex).noteOn(m_voices[voiceIndex].voiceIndex, note, velocity);
}

void MelodicInstrument::noteOff(int voiceIdx, int note, float velocity) noexcept
{
   const auto voiceIndex = voiceIdx % m_voices.size();
   soundDevice(m_voices[voiceIndex].soundDeviceIndex).noteOff(m_voices[voiceIndex].voiceIndex, note, velocity);
}

void MelodicInstrument::pitchBend(float value) noexcept
{
   for(auto& voiceDescr : m_voices)
   {
      soundDevice(voiceDescr.soundDeviceIndex).pitchBend(voiceDescr.voiceIndex, value);
   }
}

void MelodicInstrument::pitchBend(int voiceIdx, float value) noexcept
{
   const auto voiceIndex = voiceIdx % m_voices.size();
   soundDevice(m_voices[voiceIndex].soundDeviceIndex).pitchBend(m_voices[voiceIndex].voiceIndex, value);
}

void MelodicInstrument::parameterChange(int parameterId, float value) noexcept
{
   for(auto& voiceDescr : m_voices)
   {
      soundDevice(voiceDescr.soundDeviceIndex).setSoundParameterValue(voiceDescr.voiceIndex, parameterId, value);
   }
}

void MelodicInstrument::parameterChange(int voiceIdx, int parameterId, float value) noexcept
{
   const auto voiceIndex = voiceIdx % m_voices.size();
   soundDevice(m_voices[voiceIndex].soundDeviceIndex).setSoundParameterValue(m_voices[voiceIndex].voiceIndex, parameterId, value);
}