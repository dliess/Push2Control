#ifndef KIT_TRACK_H
#define KIT_TRACK_H

#include <vector>
#include "FixedSizeVector.h"
#include "Meta.h"

struct MusicDeviceHolder;

// TODO: find a namespacing solution for register_members(),
// so these classes can get a namespace
struct VoiceDescr
{
   int soundDeviceIndex;
   int voiceIndex;
   int noteOffset;
};
struct KitSound
{
   static constexpr int NUM_MAX_VOICES_PER_KIT_VOICE = 4;
   //using Voices = util::FixedSizeVector<VoiceDescr, NUM_MAX_VOICES_PER_KIT_VOICE>;
   using Voices = std::vector<VoiceDescr>;
   Voices voices;
};

class KitInstrument
{
public:
   KitInstrument(MusicDeviceHolder &rMusicDeviceHolder) noexcept;
   void noteOn(int soundIndex, int note, float velocity) noexcept;
   void noteOff(int soundIndex, int note, float velocity) noexcept;
   friend auto meta::registerMembers<KitInstrument>();
private:
   MusicDeviceHolder    &m_rMusicDeviceHolder;
   std::vector<KitSound> m_sounds;
};

#include <json.hpp>
void to_json(nlohmann::json& j, const person& p) {
   j = json{{"name", p.name}, {"address", p.address}, {"age", p.age}};
}

void from_json(const nlohmann::json& j, person& p) {
   j.at("name").get_to(p.name);
   j.at("address").get_to(p.address);
   j.at("age").get_to(p.age);
}
/*
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
        member("soundDeviceIndex", &VoiceDescr::soundDeviceIndex),
        member("voiceIndex", &VoiceDescr::voiceIndex),
        member("noteOffset", &VoiceDescr::noteOffset)
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
*/

#endif // KIT_TRACK_H