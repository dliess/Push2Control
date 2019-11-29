#include "ControllerDeviceParameterModel.h"
#include "MusicDeviceHolder.h"
#include "ParameterMapper.h"
#include "MappingCurveQt.h"
#include <iostream>

push2::qt::ControllerDeviceParameterModel::ControllerDeviceParameterModel(MusicDeviceHolder& rMusicDeviceHolder,
                                                                          ParameterMapper&   rParameterMapper,
                                                                          QObject*           parent) :
    QAbstractListModel(parent),
    m_rMusicDeviceHolder(rMusicDeviceHolder),
    m_rParameterMapper(rParameterMapper),
    m_currentCategoryIndexes(m_rMusicDeviceHolder.controllerDevices.size(), UNSELECTED)
{
    QObject::connect(&m_updateTimer, &QTimer::timeout, [this](){
        emit dataChanged(createIndex(0, 0),
                         createIndex(rowCount(QModelIndex()) , 0),
                         { Value } );       
    });
    m_updateTimer.start(30);
}

QHash<int, QByteArray> push2::qt::ControllerDeviceParameterModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[Value]    = "value";
    roles[Type]     = "type";
    roles[MappingCurveRole] = "mappingCurve";
    return roles;
}

int push2::qt::ControllerDeviceParameterModel::rowCount(const QModelIndex&) const
{
    if(UNSELECTED == m_currentDeviceIndex ||
       UNSELECTED == m_currentCategoryIndexes[m_currentDeviceIndex])
    {
        return 0;
    }
    return m_rMusicDeviceHolder.controllerDevice(m_currentDeviceIndex)->pDescr->controllerSection->parameter_categories[m_currentCategoryIndexes[m_currentDeviceIndex]].parameter_ids.size();
}

QVariant push2::qt::ControllerDeviceParameterModel::data(const QModelIndex &index, int role) const
{
    if(UNSELECTED == m_currentDeviceIndex ||
       UNSELECTED == m_currentCategoryIndexes[m_currentDeviceIndex] ||
       UNSELECTED == m_currentPresetIndex)
    {
        return QVariant();
    }
    MusicDevice& rDevice = *m_rMusicDeviceHolder.controllerDevice(m_currentDeviceIndex);
    const auto parameterIdx = parameterId(index.row());
    const auto& rParam = rDevice.pDescr->controllerSection->parameters[parameterIdx];

    switch(role)
    {
        case NameRole:
        {
            return QString(rParam.name.c_str());
        }
        case Value:
        {
            const auto value = rDevice.getControllerParameterValue(m_currentPresetIndex, parameterIdx);
            return value;
        }
        case Type:
        {
            return QString("ContinousValue");
        }
        case MappingCurveRole:
        {
            ParameterId paramId( {uint32_t(m_currentDeviceIndex), uint32_t(m_currentPresetIndex), uint32_t(parameterId(index.row()))} );
            auto valueCurve = m_rParameterMapper.getValueCurveOfControl(paramId);
            if(valueCurve)
            {
                MappingCurve mappingCurve(*valueCurve);
                return QVariant::fromValue(mappingCurve);
            }
        }
        default:
        {
            break;
        }
    }
    return QVariant();
}

int push2::qt::ControllerDeviceParameterModel::currentCategoryIndex() const
{
    if(UNSELECTED == m_currentDeviceIndex)
    {
        return UNSELECTED;
    }
    return m_currentCategoryIndexes[m_currentDeviceIndex];
}

void push2::qt::ControllerDeviceParameterModel::setCurrentCategoryIndex(int index)
{
    if(UNSELECTED == m_currentDeviceIndex)
    {
        return;
    }

    if(m_currentCategoryIndexes[m_currentDeviceIndex] != index)
    {
        QAbstractListModel::beginResetModel();
        m_currentCategoryIndexes[m_currentDeviceIndex] = index;
        if(UNSELECTED == m_currentPresetIndex && rowCount(QModelIndex()) > 0)
        {
            m_currentPresetIndex = 0;
        }
        QAbstractListModel::endResetModel();
    }
}

void push2::qt::ControllerDeviceParameterModel::incrementValueMapOutAtMaxAmount(int parameterInCategoryIndex, float increment)
{
    const auto parameterIdx = parameterId(parameterInCategoryIndex);
    ParameterId paramId( {uint32_t(m_currentDeviceIndex), uint32_t(m_currentPresetIndex), uint32_t(parameterIdx)} );
    auto valueCurve = m_rParameterMapper.getValueCurveOfControl(paramId);
    if(valueCurve)
    {
        valueCurve->setOutAtMax(valueCurve->outAtMax() + increment);
        emit dataChanged(createIndex(parameterInCategoryIndex, 0),
                         createIndex(rowCount(QModelIndex()) , 0),
                         { MappingCurveRole } );
        MusicDevice& rDevice = *m_rMusicDeviceHolder.controllerDevice(m_currentDeviceIndex);
        const auto value = rDevice.getControllerParameterValue(m_currentPresetIndex, parameterIdx);
        m_rParameterMapper.reactToControllerParameterChange(uint32_t(m_currentDeviceIndex), 
                                                            uint32_t(m_currentPresetIndex),
                                                            uint32_t(parameterIdx),
                                                            value);
    }   
}

void push2::qt::ControllerDeviceParameterModel::incrementValueMapOutAtMinAmount(int parameterInCategoryIndex, float increment)
{
    const auto parameterIdx = parameterId(parameterInCategoryIndex);
    ParameterId paramId( {uint32_t(m_currentDeviceIndex), uint32_t(m_currentPresetIndex), uint32_t(parameterIdx)} );
    auto valueCurve = m_rParameterMapper.getValueCurveOfControl(paramId);
    if(valueCurve)
    {
        valueCurve->setOutAtMin(valueCurve->outAtMin() + increment);
        emit dataChanged(createIndex(parameterInCategoryIndex, 0),
                         createIndex(rowCount(QModelIndex()) , 0),
                         { MappingCurveRole } );
        MusicDevice& rDevice = *m_rMusicDeviceHolder.controllerDevice(m_currentDeviceIndex);
        const auto value = rDevice.getControllerParameterValue(m_currentPresetIndex, parameterIdx);
        m_rParameterMapper.reactToControllerParameterChange(uint32_t(m_currentDeviceIndex), 
                                                            uint32_t(m_currentPresetIndex),
                                                            uint32_t(parameterIdx),
                                                            value);
    }   
}

void push2::qt::ControllerDeviceParameterModel::incrementValueMapBendAmount(int parameterInCategoryIndex, float increment)
{
    const auto parameterIdx = parameterId(parameterInCategoryIndex);
    ParameterId paramId( {uint32_t(m_currentDeviceIndex), uint32_t(m_currentPresetIndex), uint32_t(parameterIdx)} );
    auto valueCurve = m_rParameterMapper.getValueCurveOfControl(paramId);
    if(valueCurve)
    {
        valueCurve->setBendValue(valueCurve->bendValue() + increment);
        emit dataChanged(createIndex(parameterInCategoryIndex, 0),
                         createIndex(rowCount(QModelIndex()) , 0),
                         { MappingCurveRole } );
        MusicDevice& rDevice = *m_rMusicDeviceHolder.controllerDevice(m_currentDeviceIndex);
        const auto value = rDevice.getControllerParameterValue(m_currentPresetIndex, parameterIdx);
        m_rParameterMapper.reactToControllerParameterChange(uint32_t(m_currentDeviceIndex), 
                                                            uint32_t(m_currentPresetIndex),
                                                            uint32_t(parameterIdx),
                                                            value);
    }
}

void push2::qt::ControllerDeviceParameterModel::onCurrentDeviceIndexChanged(int index)
{
    if(m_currentDeviceIndex != index)
    {
        QAbstractListModel::beginResetModel();
        m_currentDeviceIndex = index;
        auto &contrDev = *m_rMusicDeviceHolder.controllerDevice(m_currentDeviceIndex);
        if(UNSELECTED == m_currentCategoryIndexes[m_currentDeviceIndex] &&
           contrDev.pDescr->controllerSection->parameter_categories.size() > 0)
        {
            m_currentCategoryIndexes[m_currentDeviceIndex] = 0;
        }
        QAbstractListModel::endResetModel();
    }
}

void push2::qt::ControllerDeviceParameterModel::onCurrentPresetIndexChanged(int index)
{
    if(m_currentPresetIndex != index)
    {
        QAbstractListModel::beginResetModel();
        m_currentPresetIndex = index;
        QAbstractListModel::endResetModel();
    }
}

int push2::qt::ControllerDeviceParameterModel::parameterId(int paramIdxInCategory) const
{
    return m_rMusicDeviceHolder.controllerDevice(m_currentDeviceIndex)->pDescr->controllerSection->getParamIdByCatIdx(m_currentCategoryIndexes[m_currentDeviceIndex], paramIdxInCategory);
}
