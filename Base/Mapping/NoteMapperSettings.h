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

struct ControllerDeviceId
{
   MusicDeviceId controllerDeviceId;
   std::optional<int> presetIndex;
   bool operator==(const ControllerDeviceId& rhs) const noexcept
   {
      return controllerDeviceId == rhs.controllerDeviceId && 
               presetIndex == rhs.presetIndex;
   }
   struct HashFn
   { 
      size_t operator()(const ControllerDeviceId& controllerDeviceId) const noexcept
      {
         return (std::hash<MusicDeviceId>()(controllerDeviceId.controllerDeviceId) << 4) ^
                  (controllerDeviceId.presetIndex ? *controllerDeviceId.presetIndex : 0); 
      } 
   };
};

struct NoteMapperDescription
{
   ControllerDeviceId                                      source;
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
inline auto registerMembers<ControllerDeviceId>()
{
   return members(
      member("controllerDeviceId", &ControllerDeviceId::controllerDeviceId),
      member("presetIndex",        &ControllerDeviceId::presetIndex)
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