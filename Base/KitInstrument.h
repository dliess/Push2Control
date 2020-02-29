#ifndef KIT_INSTRUMENT_H
#define KIT_INSTRUMENT_H

#include <vector>
#include "MusicDeviceId.h"
#include "MusicDeviceHolder.h"
#include "Meta.h"
#include <memory>
#include <array>

struct MusicDeviceHolder;
struct MusicDevice;

// TODO: find a namespacing solution for register_members(),
// so these classes can get a namespace
struct VoiceDescr
{
   MusicDeviceId soundDeviceId;
   std::shared_ptr<MusicDevice> pSoundDevice;
   int voiceIndex;
   int noteOffset;
   void updateMusicDevicePtr(MusicDeviceHolder &rMusicDeviceHolder) noexcept{
       auto iter = rMusicDeviceHolder.soundDevices.find(soundDeviceId);
       if(rMusicDeviceHolder.soundDevices.end() != iter)
       {
           pSoundDevice = iter->second;
       }
   }
};
struct KitSound
{
   static constexpr int NUM_MAX_VOICES_PER_KIT_VOICE = 4;
   using Voices = std::array<VoiceDescr, NUM_MAX_VOICES_PER_KIT_VOICE>;
   Voices voices;
   void init(MusicDeviceHolder &rMusicDeviceHolder) noexcept {
       for(auto& voice : voices) voice.updateMusicDevicePtr(rMusicDeviceHolder);
   }
};

class KitInstrument
{
public:
   void init(MusicDeviceHolder &rMusicDeviceHolder) noexcept;
   void noteOn(int soundIndex, int note, float velocity) noexcept;
   void noteOff(int soundIndex, int note, float velocity) noexcept;
   void addKitSound(const KitSound& kitSound) noexcept;
   void addKitSound(KitSound&& kitSound) noexcept;
   template<typename T>
   void add(T&& kitSound) noexcept {m_sounds.emplace_back(std::forward<T>(kitSound));};

   friend auto meta::registerMembers<KitInstrument>();
private:
   std::vector<KitSound> m_sounds;
};

namespace meta
{

template <>
inline auto registerMembers<KitInstrument>()
{
    return members(
        member("sounds", &KitInstrument::m_sounds)
    );
}

template <>
inline auto registerMembers<VoiceDescr>()
{
    return members(
        member("soundDeviceId", &VoiceDescr::soundDeviceId),
        member("voiceIndex",    &VoiceDescr::voiceIndex),
        member("noteOffset",    &VoiceDescr::noteOffset)
    );
}

template <>
inline auto registerMembers<KitSound>()
{
    return members(
        member("voices", &KitSound::voices)
    );
}

} // namespace meta


#endif // KIT_INSTRUMENT_H