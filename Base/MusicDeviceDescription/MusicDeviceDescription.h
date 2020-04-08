#ifndef MUSIC_DEVICE_DESCRIPTION_H
#define MUSIC_DEVICE_DESCRIPTION_H

#include "SoundSection.h"
#include "ControllerSection.h"
#include "Meta.h"
#include "JsonCast.h"
#include <mpark/variant.hpp>
#include <string>
#include <vector>
#include <optional>

struct MusicDeviceDescription
{
   std::string configPath;
   enum class Type
   {
      Unknown,
      ControllerDevice,
      SoundDevice,
      Both
   };

   std::string version;
   std::string manufacturer;
   std::string productName;
   std::optional<SoundSection>      soundSection;
   std::optional<ControllerSection> controllerSection;
   std::optional<bool> reactsToTransportCommands;

   static inline std::string type2String(Type type);
   static inline Type typeFromString(const std::string& str);
};


//----------------------------------------------------------------------------
//--------------------- ENUM <-> String conversions --------------------------
//----------------------------------------------------------------------------

inline
std::string MusicDeviceDescription::type2String(MusicDeviceDescription::Type type)
{
   switch(type)
   {
      case MusicDeviceDescription::Type::ControllerDevice: return "controller";
      case MusicDeviceDescription::Type::SoundDevice: return "sounddevice";
      case MusicDeviceDescription::Type::Both: return "both";
      case MusicDeviceDescription::Type::Unknown: [[fallthrough]];
      default: return "unknown";
   }
}

inline
MusicDeviceDescription::Type MusicDeviceDescription::typeFromString(const std::string& str)
{
   if(str == "controller") return MusicDeviceDescription::Type::ControllerDevice;
   else if(str == "sounddevice") return MusicDeviceDescription::Type::SoundDevice;
   else if(str == "both") return MusicDeviceDescription::Type::Both;
   return MusicDeviceDescription::Type::Unknown;
}

template <>
inline void to_json<MusicDeviceDescription::Type>(nlohmann::json& j, const MusicDeviceDescription::Type& obj)
{
   j = MusicDeviceDescription::type2String(obj);
}

template <>
inline void from_json<MusicDeviceDescription::Type>(const nlohmann::json& j, MusicDeviceDescription::Type& obj)
{
   obj = MusicDeviceDescription::typeFromString(j.get<std::string>());
}

//------------------------------------------------------------
//--------------------- Reflections --------------------------
//------------------------------------------------------------

namespace meta
{

template <>
inline auto registerMembers<MusicDeviceDescription>()
{
   return members(
      member("configPath",  &MusicDeviceDescription::configPath), // Just a workaround, it's automatically filled by the description loader
      member("version", &MusicDeviceDescription::version),
      member("manufacturer", &MusicDeviceDescription::manufacturer),
      member("productName", &MusicDeviceDescription::productName),
      member("soundSection", &MusicDeviceDescription::soundSection),
      member("controllerSection", &MusicDeviceDescription::controllerSection),
      member("reactsToTransportCommands", &MusicDeviceDescription::reactsToTransportCommands)
   );
}

} // namespace meta

#endif