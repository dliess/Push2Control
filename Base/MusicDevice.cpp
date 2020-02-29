#include "MusicDevice.h"
#include "JsonCast.h"
#include "loguru.hpp"
#include <iostream>

MusicDevice::MusicDevice(MusicDeviceId                           deviceId,
                         std::shared_ptr<MusicDeviceDescription> descr,
                         std::shared_ptr<SoundPresets>           soundPresets) noexcept:
    m_deviceId(std::move(deviceId)),
    pDescr(std::move(descr)),
    pSoundPresets(std::move(soundPresets))
{
    if(pDescr->soundSection)
    {
        m_soundParams.resize(pDescr->soundSection->voices.size());
        for(auto& voiceData : m_soundParams)
        {
            voiceData.parameters.resize(pDescr->soundSection->parameters.size());
        }
    }
    if(pDescr->controllerSection)
    {
        m_controlParams.resize(pDescr->controllerSection->num_presets);
        for(auto& presets : m_controlParams)
        {
            presets.resize(pDescr->controllerSection->parameters.size());
        }
    }

    if(pDescr->soundSection)
    {
        utils::Settings<MusicDevice>::load("", "Settings.json", std::string("sound-params-") + m_deviceId.toStr());
    }
}

MusicDevice::~MusicDevice() noexcept
{
    if(pDescr->soundSection)
    {
        utils::Settings<MusicDevice>::save("", "Settings.json", std::string("sound-params-") + m_deviceId.toStr());
    }
}

MusicDeviceId MusicDevice::deviceId() const noexcept
{
    return m_deviceId;
}

void MusicDevice::addMidiIn(std::unique_ptr<midi::IMidiInMedium> pMedium) noexcept
{
    if(!m_midiInMsgHandler)
    {
        m_midiInMsgHandler.emplace(std::move(pMedium), pDescr);
        m_midiInMsgHandler->registerForSoundParameter([this](uint32_t voiceId, uint32_t parameterId, float value){
            if(voiceId >= m_soundParams.size())
            {
                LOG_F(ERROR, "{} voiceId {} >= {} too big", pDescr->productName, voiceId, m_soundParams.size());
                return;
            }
            if(parameterId >= m_soundParams[voiceId].parameters.size())
            {
                LOG_F(ERROR, "{} parameterId {} >= {} too big", pDescr->productName, parameterId, m_soundParams[voiceId].parameters.size());
                return;
            }
            m_soundParams[voiceId].parameters[parameterId] = value;
        });
        m_midiInMsgHandler->registerForControlParameter([this](uint32_t presetId, uint32_t parameterId, float value){
            m_controlParams[presetId][parameterId] = value;
            for(auto& cb : m_controllerParameterChangeCbsAll)
            {
                cb(presetId, parameterId, value);
            }
        });
    }
}

void MusicDevice::addMidiOut(std::unique_ptr<midi::IMidiOutMedium> pMedium) noexcept
{
    if(m_midiOutHandler) return;
    m_midiOutHandler.emplace(std::move(pMedium), pDescr);
    if(!m_midiOutHandler->sendParameterDumpRequest())
    {
        for(int voiceIdx = 0; voiceIdx < m_soundParams.size(); ++voiceIdx)
        {
            for(int paramIdx = 0; paramIdx < m_soundParams[voiceIdx].parameters.size(); ++paramIdx)
            {
                m_midiOutHandler->sendSoundParameter(voiceIdx, 
                                                    paramIdx,
                                                    m_soundParams[voiceIdx].parameters[paramIdx]);
            }
        }
    }
}

float MusicDevice::getSoundParameterValue(uint32_t voiceId, uint32_t parameterId) const noexcept
{
    //std::cout << "getParameter voiceId: " << voiceId << " parameterId: " << parameterId << std::endl;
    return m_soundParams[voiceId].parameters[parameterId];
}

void MusicDevice::setSoundParameterValue(uint32_t voiceId, uint32_t parameterId, float value) noexcept
{
    //std::cout << "setParameter voiceId: " << voiceId << " parameterId: " << parameterId << std::endl;
    if( !pDescr->soundSection ||
        parameterId >= pDescr->soundSection->parameters.size())
    {
        return;
    }
    value = std::max(value, 0.0f);
    value = std::min(value, 1.0f);
    m_soundParams[voiceId].parameters[parameterId] = value;

    if(m_midiOutHandler) m_midiOutHandler->sendSoundParameter(voiceId, parameterId, value);
}

float MusicDevice::getControllerParameterValue(uint32_t presetId, uint32_t parameterId) const noexcept
{
    return m_controlParams[presetId][parameterId];
}

void MusicDevice::quarterNotePer24Tick() noexcept
{
    if(m_midiOutHandler)
    {
        m_midiOutHandler->quarterNotePer24Tick();
    }
}

void MusicDevice::startSequencer() noexcept
{
    if(m_midiOutHandler)
    {
        m_midiOutHandler->startSequencer();
    }
}

void MusicDevice::stopSequencer() noexcept
{
    if(m_midiOutHandler)
    {
        m_midiOutHandler->stopSequencer() ;
    }
}

std::vector<std::string> MusicDevice::getSoundPresetList() const noexcept
{
    if(!pSoundPresets) return std::move(std::vector<std::string>()); 
    return pSoundPresets->getSoundPresetList();
}

std::string MusicDevice::getActualSoundPresetName(int voiceIdx) const noexcept
{
    try{
        return m_soundParams.at(voiceIdx).actualPreset;
    }catch(std::out_of_range& e){
        LOG_F(ERROR, "{}", e.what());
        return std::string();
    }
}

void MusicDevice::registerPresetListChangeCb(std::function<void()> cb) noexcept
{
    if(!pSoundPresets) return;
    pSoundPresets->registerPresetListChangeCb(cb);
}

void MusicDevice::clearPresetListChangeCb() noexcept
{
    if(!pSoundPresets) return;
    pSoundPresets->clearPresetListChangeCb();
}

void MusicDevice::resetToActualSoundPreset(int voiceIdx) noexcept
{
    if(!pSoundPresets) return;
    try{
        const auto& preset = pSoundPresets->at(m_soundParams.at(voiceIdx).actualPreset);
        for(int paramIdx = 0; paramIdx < m_soundParams.at(voiceIdx).parameters.size(); ++paramIdx)
        {
            if(m_soundParams[voiceIdx].parameters[paramIdx] != preset[paramIdx])
            {
                m_soundParams[voiceIdx].parameters[paramIdx] = preset[paramIdx];
                if(m_midiOutHandler) m_midiOutHandler->sendSoundParameter(voiceIdx, paramIdx, preset[paramIdx]);
            }
        }
    }catch(std::out_of_range& e){
        LOG_F(ERROR, "{}", e.what());
    }
}

void MusicDevice::selectSoundPreset(int voiceIdx, const std::string& presetName) noexcept
{
    if(!pSoundPresets) return;
    if(!pSoundPresets->hasSoundPreset(presetName)) return;
    try{
        m_soundParams.at(voiceIdx).actualPreset = presetName;
        resetToActualSoundPreset(voiceIdx);
    }catch(std::out_of_range& e){
        LOG_F(ERROR, "{}", e.what());
    }
}

void MusicDevice::storeAsSoundPreset(int voiceIdx, const std::string& presetName) noexcept
{
    if(!pSoundPresets) return;
    auto& actPreset = m_soundParams.at(voiceIdx).actualPreset;
    try{
        if(!presetName.empty())
        {
            actPreset = presetName;
        }
        else
        {
            if(actPreset.empty())
            {
                actPreset = "preset";
            }
            actPreset = pSoundPresets->incrementNameIdx(actPreset);
        }
        pSoundPresets->saveSoundPreset(actPreset, m_soundParams.at(voiceIdx).parameters);
    }catch(std::out_of_range& e){
        LOG_F(ERROR, "{}", e.what());
    }
}

void MusicDevice::resetParametersToZeroState(int voiceIdx) noexcept
{
    for(int paramIdx = 0; paramIdx < m_soundParams[voiceIdx].parameters.size(); ++paramIdx)
    {
        const auto initialParamValue = getInitalParameterValue(paramIdx);
        if(m_soundParams[voiceIdx].parameters[paramIdx] != initialParamValue)
        {
            m_soundParams[voiceIdx].parameters[paramIdx] = initialParamValue;
            if(m_midiOutHandler) m_midiOutHandler->sendSoundParameter(voiceIdx, paramIdx, initialParamValue);
        }
    }
}

void MusicDevice::deleteSoundPreset(int voiceIdx, const std::string& newSelectedPreset) noexcept
{
    if(!pSoundPresets) return;
    try{
        pSoundPresets->deleteSoundPreset(m_soundParams.at(voiceIdx).actualPreset);
        selectSoundPreset(voiceIdx, newSelectedPreset);
    }catch(std::out_of_range& e){
        LOG_F(ERROR, "{}", e.what());
    }
}

void MusicDevice::noteOn(int voiceIndex, int note, float velocity) noexcept
{
    if(m_midiOutHandler)
    {
        m_midiOutHandler->noteOn(voiceIndex, note, velocity);
    }
}

void MusicDevice::noteOff(int voiceIndex, int note, float velocity) noexcept
{
    if(m_midiOutHandler)
    {
        m_midiOutHandler->noteOff(voiceIndex, note, velocity);
    }
}

void MusicDevice::pitchBend(int voiceIndex, float value) noexcept
{
    if(m_midiOutHandler)
    {
        m_midiOutHandler->pitchBend(voiceIndex, value);
    }
}

void MusicDevice::registerForControllerParameterChange(ParameterChangeCb cb) noexcept
{
    m_controllerParameterChangeCbsAll.push_back(cb);
}

void MusicDevice::processMidiOutBuffers() noexcept
{
    if(m_midiOutHandler) m_midiOutHandler->processMidiOutBuffers();
}


float MusicDevice::getInitalParameterValue(uint32_t parameterId) const noexcept
{
    // TODO: doit
    return 0.0;
}
