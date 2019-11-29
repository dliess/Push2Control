#ifndef PARAMETER_MAPPER_H
#define PARAMETER_MAPPER_H

#include <optional>
#include <unordered_map>
#include <tuple>
#include "ParameterId.h"
#include "ValueCurve.h"
#include "Settings.h"

struct MusicDeviceHolder;


// ============== Settings ===============

struct SettingsParameterId
{
   std::string musicDevice;
   uint32_t    voiceId;
   uint32_t    parameterId;
};

struct SettingsMappingDescription
{
   SettingsParameterId srcParam;
   SettingsParameterId destParam;
   ValueCurve          valueCurve;
};

namespace meta
{

template <>
inline auto registerMembers<SettingsParameterId>()
{
   return members(
      member("musicDevice", &SettingsParameterId::musicDevice),
      member("voiceId",     &SettingsParameterId::voiceId),
      member("parameterId", &SettingsParameterId::parameterId)
   );
}

template <>
inline auto registerMembers<ValueCurve>()
{
   return members(
      member("m_outAtMax", &ValueCurve::outAtMax, &ValueCurve::setOutAtMax),
      member("m_outAtMin", &ValueCurve::outAtMin, &ValueCurve::setOutAtMin),
      member("m_bendValue", &ValueCurve::bendValue, &ValueCurve::setBendValue)
   );
}

template <>
inline auto registerMembers<SettingsMappingDescription>()
{
   return members(
      member("srcParam", &SettingsMappingDescription::srcParam),
      member("destParam", &SettingsMappingDescription::destParam),
      member("valueCurve", &SettingsMappingDescription::valueCurve)
   );
}

} // namespace meta
// =======================================


class ParameterMapper : public utils::Settings<ParameterMapper>
{
public:
   enum class MapBehaviour{
      Normal,
      ModulateIncrease,
      ModulateDecrease
   };
   ParameterMapper(MusicDeviceHolder& rMusicDeviceHolder) noexcept;
   void setMappingDestinationCandidate(const ParameterId& parameterId, MapBehaviour mapBehaviour = MapBehaviour::Normal) noexcept;
   std::optional<ParameterId> getMappingDestinationCandidate() const noexcept;
   std::optional<ParameterId> getControllerMappedTo(const ParameterId& paramId) const noexcept;
   std::optional<ParameterId> getSoundDeviceOfControl(const ParameterId& paramId) const noexcept;
   ValueCurve* getValueCurveOfControl(const ParameterId& paramId) noexcept;
   void reactToControllerParameterChange(uint32_t ctrlDevIdx, uint32_t presetId, uint32_t parameterId, float value);

   // ============== Settings ===============
   using Settings = std::vector<SettingsMappingDescription>;
   Settings getSettings() const noexcept;
   void setSettings(const Settings& settings) noexcept;
   // =======================================

private:
   MusicDeviceHolder                                  &m_rMusicDeviceHolder;
   std::optional<std::pair<ParameterId, MapBehaviour>> m_mappingDestinationCandidate;
   struct MappingDescription
   {
      ParameterId destParam;
      ValueCurve  valueCurve;
   };
   std::unordered_map<ParameterId, MappingDescription, ParameterId::HashFn> m_parameterMap;
};

#endif