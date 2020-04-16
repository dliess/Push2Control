#ifndef MELODIC_INSTRUMENT_H
#define MELODIC_INSTRUMENT_H

#include <vector>
#include <array>
#include "MusicDeviceHolder.h"
#include "MusicDeviceId.h"
#include "Meta.h"

class MusicDevice;

struct MelodicInstrumentVoice
{
   MusicDeviceId                soundDeviceId;
   std::shared_ptr<MusicDevice> pSoundDevice;
   int                          voiceIndex;
};

class MelodicInstrument
{
public:
   void init(MusicDeviceHolder &rMusicDeviceHolder) noexcept;
   void noteOn(int note, float velocity) noexcept;
   void noteOff(int note, float velocity) noexcept;
   void pitchBend(float value) noexcept;
   void parameterChange(int parameterId, float value) noexcept;

   void noteOn(int voiceIdx, int note, float velocity) noexcept;
   void noteOff(int voiceIdx, int note, float velocity) noexcept;
   void pitchBend(int voiceIdx, float value) noexcept;
   void parameterChange(int voiceIdx, int parameterId, float value) noexcept;

   template<typename T>
   void addVoice(T&& voice) noexcept {m_voices.emplace_back(std::forward<T>(voice));};

   std::string name() const noexcept;
   void setName(const std::string& name) noexcept;

   using VoiceContainer = std::vector<MelodicInstrumentVoice>;
   VoiceContainer &voices() noexcept;
   friend auto meta::registerMembers<MelodicInstrument>();
private:
   std::string              m_name;
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
};

namespace meta
{

template <>
inline auto registerMembers<MelodicInstrumentVoice>()
{
    return members(
        member("soundDeviceId", &MelodicInstrumentVoice::soundDeviceId),
        member("voiceIndex", &MelodicInstrumentVoice::voiceIndex)
    );
}

template <>
inline auto registerMembers<MelodicInstrument>()
{
    return members(
        member("voices", &MelodicInstrument::m_voices)
    );
}

} // namespace meta

#endif // MELODIC_INSTRUMENT_H