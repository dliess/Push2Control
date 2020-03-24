#ifndef MOTE_MAPPER_SETTINGS_H
#define MOTE_MAPPER_SETTINGS_H

#include <string>
#include <optional>
#include <mpark/variant.hpp>
#include "Settings.h"
#include "MusicDeviceId.h"

// ============== Settings ===============

struct KitSoundIdSettings
{
   std::string kitInstrumentId;
   int         soundIndex{0};
};

struct MelodicInstrumentIdSettings
{
   std::string melodicInstrumentId;
};

struct ControllerDeviceIdSettings
{
   MusicDeviceId controllerDeviceId;
   std::optional<int> presetIndex;
};

struct NoteMapperDescription
{
   ControllerDeviceIdSettings                                      source;
   mpark::variant<KitSoundIdSettings, MelodicInstrumentIdSettings> destination;
};

namespace meta
{

template <>
inline auto registerMembers<KitSoundIdSettings>()
{
   return members(
      member("kitInstrumentId", &KitSoundIdSettings::kitInstrumentId),
      member("soundIndex",      &KitSoundIdSettings::soundIndex)
   );
}

template <>
inline auto registerMembers<MelodicInstrumentIdSettings>()
{
   return members(
      member("melodicInstrumentId", &MelodicInstrumentIdSettings::melodicInstrumentId)
   );
}

template <>
inline auto registerMembers<ControllerDeviceIdSettings>()
{
   return members(
      member("controllerDeviceId", &ControllerDeviceIdSettings::controllerDeviceId),
      member("presetIndex",        &ControllerDeviceIdSettings::presetIndex)
   );
}

template <>
inline auto registerMembers<NoteMapperDescription>()
{
   return members(
      member("source",      &NoteMapperDescription::source),
      member("destination", &NoteMapperDescription::destination)
   );
}

} // namespace meta

#endif // MOTE_MAPPER_SETTINGS_H