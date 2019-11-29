#include "SoundDeviceParameter.h"
#include "MusicDeviceHolder.h"
#include "ParameterMapper.h"

push2::qt::SoundDeviceParameter::SoundDeviceParameter(MusicDeviceHolder& rMusicDeviceHolder,
                                                      ParameterMapper&   rParameterMapper) :
    QObject(),
    m_parameterCategoryModel(rMusicDeviceHolder),
    m_parameterModel(rMusicDeviceHolder, rParameterMapper),
    m_soundPresetModel(rMusicDeviceHolder.soundDevices)
{
}

QAbstractListModel* push2::qt::SoundDeviceParameter::parameterCategoryModel()
{
    return &m_parameterCategoryModel;
}

QAbstractListModel* push2::qt::SoundDeviceParameter::parameterModel()
{
    return &m_parameterModel;
}

QAbstractListModel* push2::qt::SoundDeviceParameter::soundPresetModel()
{
    return &m_soundPresetModel;
}

int push2::qt::SoundDeviceParameter::currentCategoryIndex() const
{
    return m_parameterModel.currentCategoryIndex();
}

void push2::qt::SoundDeviceParameter::setCurrentCategoryIndex(int index)
{
    m_parameterModel.setCurrentCategoryIndex(index);
}

void push2::qt::SoundDeviceParameter::incrementParameterValue(int paramIdxInCategory, int increment)
{ 
    m_parameterModel.incrementParameterValue(paramIdxInCategory, increment);
}

void push2::qt::SoundDeviceParameter::learnControl(int paramIdxInCategory)
{
    m_parameterModel.learnControl(paramIdxInCategory);
}

void push2::qt::SoundDeviceParameter::onCurrentDeviceIndexChanged(int index)
{
    m_parameterCategoryModel.onCurrentDeviceIndexChanged(index);
    m_parameterModel.onCurrentDeviceIndexChanged(index);
    m_soundPresetModel.onCurrentDeviceIndexChanged(index);
}

void push2::qt::SoundDeviceParameter::onCurrentVoiceIndexChanged(int index)
{
    m_parameterModel.onCurrentVoiceIndexChanged(index);
    m_soundPresetModel.onCurrentVoiceIndexChanged(index);
}
