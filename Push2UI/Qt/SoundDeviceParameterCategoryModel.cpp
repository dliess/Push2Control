#include "SoundDeviceParameterCategoryModel.h"
#include <iostream>

push2::qt::SoundDeviceParameterCategoryModel::SoundDeviceParameterCategoryModel(MusicDeviceHolder& rMidiOpenedPorts, 
                                                                                QObject*            parent) :
    QAbstractListModel(parent),
    m_rMusicDeviceHolder(rMidiOpenedPorts)
{
}

QHash<int, QByteArray> push2::qt::SoundDeviceParameterCategoryModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}

int push2::qt::SoundDeviceParameterCategoryModel::rowCount(const QModelIndex&) const
{
    if(UNSELECTED == m_currentDeviceIndex)
    {
        return 0;
    }
    return m_rMusicDeviceHolder.soundDevice(m_currentDeviceIndex)->pDescr->soundSection->parameter_categories.size();
}

QVariant push2::qt::SoundDeviceParameterCategoryModel::data(const QModelIndex &index, int role) const
{
    const auto idx = index.row();
    switch(role)
    {
        case NameRole:
        {
            if(UNSELECTED != m_currentDeviceIndex)
            {
                return QString(m_rMusicDeviceHolder.soundDevice(m_currentDeviceIndex)->pDescr->soundSection->parameter_categories[idx].category_name.c_str());
            }
        }
        default:
        {
            break;
        }
    }
    return QVariant();
}

void push2::qt::SoundDeviceParameterCategoryModel::onCurrentDeviceIndexChanged(int index)
{
    if(m_currentDeviceIndex != index)
    {
        QAbstractListModel::beginResetModel();
        m_currentDeviceIndex = index;
        QAbstractListModel::endResetModel();
    }
}