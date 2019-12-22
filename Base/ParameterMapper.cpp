#include "ParameterMapper.h"
#include "MusicDeviceHolder.h"

//#include <iostream>

ParameterMapper::ParameterMapper(MusicDeviceHolder& rMusicDeviceHolder) noexcept :
   m_rMusicDeviceHolder(rMusicDeviceHolder)
{
   for(unsigned ctrlDevIdx = 0; ctrlDevIdx < rMusicDeviceHolder.controllerDevices.size(); ++ctrlDevIdx)
   {
      auto &device = rMusicDeviceHolder.controllerDevices.atIdx(ctrlDevIdx).second;
      device->registerForControllerParameterChange([this, ctrlDevIdx, &rMusicDeviceHolder](uint32_t presetId, uint32_t parameterId, float value){
         if(m_mappingDestinationCandidate)
         {
            //std::cout << "create mapping: " << ParameterId({ctrlDevIdx, presetId, parameterId}).toString() << " -> " <<  m_mappingDestinationCandidate->toString() << std::endl;
            ParameterId source { ctrlDevIdx, presetId, parameterId };
            m_parameterMap[source] = {m_mappingDestinationCandidate->first, ValueCurve()};
            switch(m_mappingDestinationCandidate->second)
            {
               case MapBehaviour::Normal:
               {
                  break;
               }
               case MapBehaviour::ModulateIncrease:
               {
                  auto& soundDev = *rMusicDeviceHolder.soundDevices.atIdx(m_mappingDestinationCandidate->first.musicDeviceIdx).second.get();
                  soundDev.registerForControllerParameterChange([this, &source](uint32_t voiceId, uint32_t parameterId, float value){
                     if(voiceId     == m_mappingDestinationCandidate->first.voiceId && 
                        parameterId == m_mappingDestinationCandidate->first.parameterId)
                     {
                        m_parameterMap[source].valueCurve.setOutAtMin(value);
                     }
                  });
                  break;
               }
               case MapBehaviour::ModulateDecrease:
               {
                  break;
               }
               default:
               {
                  break;
               }
            }
            m_mappingDestinationCandidate.reset();
         }
         else
         {
            reactToControllerParameterChange(ctrlDevIdx, presetId, parameterId, value);
         }
      });
   }
}

void ParameterMapper::setMappingDestinationCandidate(const ParameterId& parameterId, MapBehaviour mapBehaviour) noexcept
{
   //std::cout << "emplacing (" << parameterId.toString() << std::endl;
   m_mappingDestinationCandidate.emplace(parameterId, mapBehaviour);
}

std::optional<ParameterId> ParameterMapper::getMappingDestinationCandidate() const noexcept
{
   if(m_mappingDestinationCandidate) return m_mappingDestinationCandidate->first;
   return std::nullopt;
}

std::optional<ParameterId> ParameterMapper::getControllerMappedTo(const ParameterId& paramId) const noexcept
{ // TODO: optimize with second reversed map?
   for(auto it = m_parameterMap.begin(); it != m_parameterMap.end(); ++it)
   {
      if(paramId == it->second.destParam)
      {
         std::make_optional<ParameterId>(it->first);
      }
   }
   return std::nullopt;
}

std::optional<ParameterId> ParameterMapper::getSoundDeviceOfControl(const ParameterId& paramId) const noexcept
{
   const auto it = m_parameterMap.find(paramId);
   if(it != m_parameterMap.end())
   {
      return std::make_optional<ParameterId>(it->second.destParam);
   }
   return std::nullopt;
}

ValueCurve* ParameterMapper::getValueCurveOfControl(const ParameterId& paramId) noexcept
{
   const auto it = m_parameterMap.find(paramId);
   if(it != m_parameterMap.end())
   {
      return &(it->second.valueCurve);
   }
   return nullptr;
}

void ParameterMapper::reactToControllerParameterChange(uint32_t ctrlDevIdx, uint32_t presetId, uint32_t parameterId, float value)
{
   auto it = m_parameterMap.find(ParameterId({ctrlDevIdx, presetId, parameterId}));
   if(it != m_parameterMap.end())
   {
      const auto& mappingDescr = it->second;
      //std::cout << "mapping active: " << ParameterId({ctrlDevIdx, voiceId, parameterId}).toString() << " -> " << mappingDescr.toString() << std::endl;
      auto& destDevice = *m_rMusicDeviceHolder.soundDevices.atIdx(mappingDescr.destParam.musicDeviceIdx).second;
      //if(destDevice.pDescr->controllerSection.)
      destDevice.setSoundParameterValue(mappingDescr.destParam.voiceId,
                                        mappingDescr.destParam.parameterId,
                                        mappingDescr.valueCurve.calcOutputValue(value));
   }
}

ParameterMapper::Settings ParameterMapper::getSettings() const noexcept
{
   Settings settings;
   for(auto& [id, mapping] : m_parameterMap)
   {
      const auto srcName = m_rMusicDeviceHolder.controllerDevices.atIdx(id.musicDeviceIdx).first.toStr();
      const auto dstName = m_rMusicDeviceHolder.soundDevices.atIdx(mapping.destParam.musicDeviceIdx).first.toStr();
      settings.emplace_back(SettingsMappingDescription{ {srcName, id.voiceId, id.parameterId},
                                                        {dstName, mapping.destParam.voiceId, mapping.destParam.parameterId},
                                                         mapping.valueCurve });
   }
   return settings;
}

void ParameterMapper::setSettings(const Settings& settings) noexcept
{
   for(auto& e : settings)
   {
      const auto srcIdx = m_rMusicDeviceHolder.controllerDevices.getIndexOf(e.srcParam.musicDevice);
      if(-1 == srcIdx) continue;
      const auto dstIdx = m_rMusicDeviceHolder.soundDevices.getIndexOf(e.destParam.musicDevice);
      if(-1 == dstIdx) continue;

      const ParameterId source{uint32_t(srcIdx), e.srcParam.voiceId, e.srcParam.parameterId};
      const ParameterId destination{uint32_t(dstIdx), e.destParam.voiceId, e.destParam.parameterId};
      m_parameterMap[source] = {destination, e.valueCurve};
   }
}
