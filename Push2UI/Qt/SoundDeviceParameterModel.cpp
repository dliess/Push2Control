#include "SoundDeviceParameterModel.h"
#include "MusicDeviceHolder.h"
#include "ParameterMapper.h"
#include "MappingCurveQt.h"
#include <iostream>

push2::qt::SoundDeviceParameterModel::SoundDeviceParameterModel(MusicDeviceHolder& rMusicDeviceHolder,
                                                                ParameterMapper&   rParameterMapper,
                                                                QObject*           parent) :
    QAbstractListModel(parent),
    m_rMusicDeviceHolder(rMusicDeviceHolder),
    m_rParameterMapper(rParameterMapper),
    m_currentCategoryIndexes(m_rMusicDeviceHolder.soundDevices.size(), UNSELECTED)
{
    QObject::connect(&m_updateTimer, &QTimer::timeout, [this](){
        emit dataChanged(createIndex(0, 0),
                         createIndex(rowCount(QModelIndex()) , 0),
                         { Value, ExtControlLearnState } );       
    });
    m_updateTimer.start(10);
}

QHash<int, QByteArray> push2::qt::SoundDeviceParameterModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[Value]    = "value";
    roles[Type]     = "type";
    roles[MappingCurveRole] = "mappingCurve";
    roles[ExtControlLearnState] = "extControlLearnState";
    return roles;
}

int push2::qt::SoundDeviceParameterModel::rowCount(const QModelIndex&) const
{
    if(UNSELECTED == m_currentDeviceIndex ||
       UNSELECTED == m_currentCategoryIndexes[m_currentDeviceIndex])
    {
        return 0;
    }
    MusicDevice& rDevice = *m_rMusicDeviceHolder.soundDevice(m_currentDeviceIndex);
    return rDevice.pDescr->soundSection->parameter_categories[m_currentCategoryIndexes[m_currentDeviceIndex]].parameter_ids.size();
}

QVariant push2::qt::SoundDeviceParameterModel::data(const QModelIndex &index, int role) const
{
    if(UNSELECTED == m_currentDeviceIndex ||
       UNSELECTED == m_currentCategoryIndexes[m_currentDeviceIndex] ||
       UNSELECTED == m_currentVoiceIndex)
    {
        return QVariant();
    }
    MusicDevice& rDevice = *m_rMusicDeviceHolder.soundDevice(m_currentDeviceIndex);
    const int parameterIdx = parameterId(index.row());
    const auto& rParam = rDevice.pDescr->soundSection->parameters[parameterIdx];

    switch(role)
    {
        case NameRole:
        {
            return QString(rParam.name.c_str());
        }
        case Value:
        {
            const auto value = rDevice.getSoundParameterValue(m_currentVoiceIndex, parameterIdx);
            if(SoundDeviceParameter::Type::List == rParam.type)
            {
                if(!rParam.ranges) return value;
                for(auto& e : *rParam.ranges)
                {
                    const auto min = e.range[0];
                    const auto max = e.range[1];
                    if(min <= int(value * 128) && int(value * 128) <= max)
                    {
                        return QString(e.name.c_str());
                    }
                }
                return QString("?unknown?");
            }
            else
            {
                return value;
            }
        }
        case Type:
        {
            return QString(SoundDeviceParameter::type2String(rParam.type).c_str());
        }
        case MappingCurveRole:
        {
            ParameterId paramId( {uint32_t(m_currentDeviceIndex), uint32_t(m_currentVoiceIndex), uint32_t(parameterId(index.row()))} );
            auto valueCurve = m_rParameterMapper.getValueCurveOfControl(paramId);
            if(valueCurve)
            {
                MappingCurve mappingCurve(*valueCurve);
                return QVariant::fromValue(mappingCurve);
            }
            break;
        }
        case ExtControlLearnState:
        {
            auto destCandidate = m_rParameterMapper.getMappingDestinationCandidate();
            if(!destCandidate)
            {
                return false;
            }
            return ParameterId({uint32_t(m_currentDeviceIndex), uint32_t(m_currentVoiceIndex), uint32_t(parameterId(index.row()))}) == *destCandidate;
        }
        default:
        {
            break;
        }
    }
    return QVariant();
}

int push2::qt::SoundDeviceParameterModel::currentCategoryIndex() const
{
    if(UNSELECTED == m_currentDeviceIndex)
    {
        return UNSELECTED;
    }
    return m_currentCategoryIndexes[m_currentDeviceIndex];
}

void push2::qt::SoundDeviceParameterModel::setCurrentCategoryIndex(int index)
{
    if(UNSELECTED == m_currentDeviceIndex)
    {
        return;
    }

    if(m_currentCategoryIndexes[m_currentDeviceIndex] != index)
    {
        QAbstractListModel::beginResetModel();
        m_currentCategoryIndexes[m_currentDeviceIndex] = index;
        if(UNSELECTED == m_currentVoiceIndex && rowCount(QModelIndex()) > 0)
        {
            m_currentVoiceIndex = 0;
        }
        QAbstractListModel::endResetModel();
    }
}

void push2::qt::SoundDeviceParameterModel::onCurrentDeviceIndexChanged(int index)
{
    if(m_currentDeviceIndex != index)
    {
        QAbstractListModel::beginResetModel();
        m_currentDeviceIndex = index;
        if(UNSELECTED == m_currentCategoryIndexes[m_currentDeviceIndex] &&
           m_rMusicDeviceHolder.soundDevice(m_currentDeviceIndex)->pDescr->soundSection->parameter_categories.size() > 0)
        {
            m_currentCategoryIndexes[m_currentDeviceIndex] = 0;
        }
        QAbstractListModel::endResetModel();
    }
}

void push2::qt::SoundDeviceParameterModel::onCurrentVoiceIndexChanged(int index)
{
    if(m_currentVoiceIndex != index)
    {
        QAbstractListModel::beginResetModel();
        m_currentVoiceIndex = index;
        QAbstractListModel::endResetModel();
    }
}

void push2::qt::SoundDeviceParameterModel::incrementParameterValue(int paramIdxInCategory, int increment)
{ // TODO: make this nicer
    if(UNSELECTED == m_currentDeviceIndex ||
       UNSELECTED == m_currentCategoryIndexes[m_currentDeviceIndex] ||
       UNSELECTED == m_currentVoiceIndex)
    {
        return;
    }

    const auto paramIndex = parameterId(paramIdxInCategory);
    auto &soundDev = *m_rMusicDeviceHolder.soundDevice(m_currentDeviceIndex);
    const auto val = soundDev.getSoundParameterValue(m_currentVoiceIndex, paramIndex) + (increment / 128.0);
    soundDev.setSoundParameterValue(m_currentVoiceIndex, paramIndex, val);
}

void push2::qt::SoundDeviceParameterModel::learnControl(int paramIdxInCategory)
{
    if(UNSELECTED == m_currentDeviceIndex ||
       UNSELECTED == m_currentCategoryIndexes[m_currentDeviceIndex] ||
       UNSELECTED == m_currentVoiceIndex)
    {
        return;
    }
    m_rParameterMapper.setMappingDestinationCandidate({uint32_t(m_currentDeviceIndex), uint32_t(m_currentVoiceIndex), uint32_t(parameterId(paramIdxInCategory))});
}

int push2::qt::SoundDeviceParameterModel::parameterId(int paramIdxInCategory) const
{
    return m_rMusicDeviceHolder.soundDevice(m_currentDeviceIndex)->pDescr->soundSection->getParamIdByCatIdx(m_currentCategoryIndexes[m_currentDeviceIndex], paramIdxInCategory);
}
