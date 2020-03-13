#ifndef MUSIC_DEVICE_DESCRIPTION_H
#define MUSIC_DEVICE_DESCRIPTION_H

#include "Meta.h"
#include "JsonCast.h"
#include <string>
#include <vector>
#include <optional>

struct MusicDeviceVoice
{
   std::string            name;
};

struct MusicDeviceParameterMidi
{
   std::vector<uint8_t> cc;
   std::optional<std::array<uint8_t, 2>> nrpn;
};

struct MusicDeviceParameterRange
{
   std::string name;
   std::array<float, 2> range;
};

struct SoundDeviceParameter
{
   enum class Type
   {
      Continous,
      ContinousBipolar,
      List
   };
   Type        type;
   std::string name;
   std::optional<std::string> description;
   MusicDeviceParameterMidi midi;
   std::optional<std::vector<MusicDeviceParameterRange>> ranges;

   static inline std::string type2String(Type type);
   static inline Type typeFromString(const std::string& str);
};

struct ControllerDeviceContinousControl
{
   std::string                name;
   std::optional<std::string> description;
   MusicDeviceParameterMidi   midi;
};

struct ControllerDeviceEncoder
{

};

struct ControllerDeviceButton
{

};

struct MpeDescription
{
};
struct ControllerDeviceKeyboard
{
   bool hasAftertouch;
   std::optional<MpeDescription> mpeDescription;
};

struct MusicDeviceParameterCategory
{
   std::string      category_name;
   std::vector<int> parameter_ids;
};

struct MidiCCAndValue
{
   std::vector<uint8_t> cc;
   uint8_t value;
};

struct MusicDeviceParameterDumpRequest
{
   std::optional<std::vector<uint8_t>> midi_sysex;
   std::optional<MidiCCAndValue>       midi_cc;
};

struct SoundSection
{
   enum class DefaultInstrumentType{
      DrumKit,
      InstrumentPerVoice,
      OnePolyphonicInstrument
   };
   DefaultInstrumentType                          defaultInstrumentType;
   std::vector<MusicDeviceVoice>                  voices;
   std::vector<SoundDeviceParameter>              parameters;
   std::vector<MusicDeviceParameterCategory>      parameter_categories;
   std::optional<MusicDeviceParameterDumpRequest> parameter_dump_request;
   std::optional<float>                           pitchBendFactor;
   int getParamIdByCatIdx(int catIdx, int paramInCatIdx) const{
      return parameter_categories[catIdx].parameter_ids[paramInCatIdx];
   }
   static inline std::string defaultInstrumentType2String(DefaultInstrumentType type);
   static inline DefaultInstrumentType defaultInstrumentTypeFromString(const std::string& str);
};

struct ControllerSection
{
   unsigned                                      num_presets;
   std::vector<ControllerDeviceContinousControl> parameters;
   std::vector<MusicDeviceParameterCategory>     parameter_categories;
   int getParamIdByCatIdx(int catIdx, int paramInCatIdx) const{
      return parameter_categories[catIdx].parameter_ids[paramInCatIdx];
   }
};

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
std::string SoundDeviceParameter::type2String(SoundDeviceParameter::Type type)
{
   switch(type)
   {
      case SoundDeviceParameter::Type::Continous: return "continous";
      case SoundDeviceParameter::Type::ContinousBipolar: return "continous-bipolar";
      case SoundDeviceParameter::Type::List: return "list";
      default: return "unknown";
   }
}

inline
SoundDeviceParameter::Type SoundDeviceParameter::typeFromString(const std::string& str)
{
   if(str == "continous") return SoundDeviceParameter::Type::Continous;
   else if(str == "continous-bipolar") return SoundDeviceParameter::Type::ContinousBipolar;
   else if(str == "list") return SoundDeviceParameter::Type::List;
   return SoundDeviceParameter::Type::Continous;
}

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

inline 
std::string SoundSection::defaultInstrumentType2String(DefaultInstrumentType type)
{
   switch(type)
   {
      case SoundSection::DefaultInstrumentType::DrumKit: return "DrumKit";
      case SoundSection::DefaultInstrumentType::InstrumentPerVoice : return "InstrumentPerVoice";
      case SoundSection::DefaultInstrumentType::OnePolyphonicInstrument : return "OnePolyphonicInstrument";
      default: return "unknown";
   }
}

inline 
SoundSection::DefaultInstrumentType SoundSection::defaultInstrumentTypeFromString(const std::string& str)
{
   if(str == "DrumKit") return SoundSection::DefaultInstrumentType::DrumKit;
   else if(str == "InstrumentPerVoice") return SoundSection::DefaultInstrumentType::InstrumentPerVoice;
   else if(str == "OnePolyphonicInstrument") return SoundSection::DefaultInstrumentType::OnePolyphonicInstrument;
   return SoundSection::DefaultInstrumentType::InstrumentPerVoice;
}

template <>
inline void to_json<SoundDeviceParameter::Type>(nlohmann::json& j, const SoundDeviceParameter::Type& obj)
{
   j = SoundDeviceParameter::type2String(obj);
}

template <>
inline void from_json<SoundDeviceParameter::Type>(const nlohmann::json& j, SoundDeviceParameter::Type& obj)
{
   obj = SoundDeviceParameter::typeFromString(j.get<std::string>());
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

template <>
inline void to_json<SoundSection::DefaultInstrumentType>(nlohmann::json& j, const SoundSection::DefaultInstrumentType& obj)
{
   j = SoundSection::defaultInstrumentType2String(obj);
}

template <>
inline void from_json<SoundSection::DefaultInstrumentType>(const nlohmann::json& j, SoundSection::DefaultInstrumentType& obj)
{
   obj = SoundSection::defaultInstrumentTypeFromString(j.get<std::string>());
}

//------------------------------------------------------------
//--------------------- Reflections --------------------------
//------------------------------------------------------------

namespace meta
{

template <>
inline auto registerMembers<MusicDeviceVoice>()
{
   return members(
      member("name", &MusicDeviceVoice::name)
   );
}


template <>
inline auto registerMembers<MusicDeviceParameterMidi>()
{
   return members(
      member("cc", &MusicDeviceParameterMidi::cc)
   );
}

template <>
inline auto registerMembers<MusicDeviceParameterRange>()
{
   return members(
      member("name", &MusicDeviceParameterRange::name),
      member("range", &MusicDeviceParameterRange::range)
   );
}

template <>
inline auto registerMembers<SoundDeviceParameter>()
{
   return members(
      member("name", &SoundDeviceParameter::name),
      member("description", &SoundDeviceParameter::description),
      member("type", &SoundDeviceParameter::type),
      member("midi", &SoundDeviceParameter::midi),
      member("ranges", &SoundDeviceParameter::ranges)
   );
}

template <>
inline auto registerMembers<ControllerDeviceContinousControl>()
{
   return members(
      member("name", &ControllerDeviceContinousControl::name),
      member("description", &ControllerDeviceContinousControl::description),
      member("midi", &ControllerDeviceContinousControl::midi)
   );
}

template <>
inline auto registerMembers<MusicDeviceParameterCategory>()
{
   return members(
      member("category_name", &MusicDeviceParameterCategory::category_name),
      member("parameter_ids", &MusicDeviceParameterCategory::parameter_ids)
   );
}

template <>
inline auto registerMembers<MidiCCAndValue>()
{
   return members(
      member("cc", &MidiCCAndValue::cc),
      member("value", &MidiCCAndValue::value)
   );
}

template <>
inline auto registerMembers<MusicDeviceParameterDumpRequest>()
{
   return members(
      member("midi_sysex", &MusicDeviceParameterDumpRequest::midi_sysex),
      member("midi_cc", &MusicDeviceParameterDumpRequest::midi_cc)
   );
}

template <>
inline auto registerMembers<SoundSection>()
{
   return members(
      member("defaultInstrumentType", &SoundSection::defaultInstrumentType),
      member("voices", &SoundSection::voices),
      member("parameters", &SoundSection::parameters),
      member("parameter_categories", &SoundSection::parameter_categories),
      member("parameter_dump_request", &SoundSection::parameter_dump_request)
   );
}

template <>
inline auto registerMembers<ControllerSection>()
{
   return members(
      member("num_presets", &ControllerSection::num_presets),
      member("parameters", &ControllerSection::parameters),
      member("parameter_categories", &ControllerSection::parameter_categories)
   );
}

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