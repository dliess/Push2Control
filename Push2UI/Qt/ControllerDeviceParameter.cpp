#include "ControllerDeviceParameter.h"
#include "MusicDeviceHolder.h"
#include "ParameterMapper.h"

push2::qt::ControllerDeviceParameter::ControllerDeviceParameter(MusicDeviceHolder& rMusicDeviceHolder,
                                                                ParameterMapper&   rParameterMapper) :
    QObject(),
    m_parameterCategoryModel(rMusicDeviceHolder),
    m_parameterModel(rMusicDeviceHolder, rParameterMapper)
{
}

QAbstractListModel* push2::qt::ControllerDeviceParameter::parameterCategoryModel()
{
    return &m_parameterCategoryModel;
}

QAbstractListModel* push2::qt::ControllerDeviceParameter::parameterModel()
{
    return &m_parameterModel;
}

int push2::qt::ControllerDeviceParameter::currentCategoryIndex() const
{
    return m_parameterModel.currentCategoryIndex();
}

void push2::qt::ControllerDeviceParameter::setCurrentCategoryIndex(int index)
{
    m_parameterModel.setCurrentCategoryIndex(index);
}

int push2::qt::ControllerDeviceParameter::currentParameterInCategoryIndex() const
{
    return m_currentParameterInCategoryIndex;
}

void push2::qt::ControllerDeviceParameter::setCurrentParameterInCategoryIndex(int index)
{
    m_currentParameterInCategoryIndex = index;
}

void push2::qt::ControllerDeviceParameter::incrementValueMapOutAtMaxAmount(int parameterInCategoryIndex, float increment)
{
    m_parameterModel.incrementValueMapOutAtMaxAmount(parameterInCategoryIndex, increment);
}

void push2::qt::ControllerDeviceParameter::incrementValueMapOutAtMinAmount(int parameterInCategoryIndex, float increment)
{
    m_parameterModel.incrementValueMapOutAtMinAmount(parameterInCategoryIndex, increment);
}

void push2::qt::ControllerDeviceParameter::incrementValueMapBendAmount(int parameterInCategoryIndex, qreal increment)
{
    m_parameterModel.incrementValueMapBendAmount(parameterInCategoryIndex, increment);
}

void push2::qt::ControllerDeviceParameter::onCurrentDeviceIndexChanged(int index)
{
    m_parameterCategoryModel.onCurrentDeviceIndexChanged(index);
    m_parameterModel.onCurrentDeviceIndexChanged(index);
}

void push2::qt::ControllerDeviceParameter::onCurrentPresetIndexChanged(int index)
{
    m_parameterModel.onCurrentPresetIndexChanged(index);
}
