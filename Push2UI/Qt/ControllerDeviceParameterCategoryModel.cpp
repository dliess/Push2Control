#include "ControllerDeviceParameterCategoryModel.h"
#include <iostream>

push2::qt::ControllerDeviceParameterCategoryModel::ControllerDeviceParameterCategoryModel(MusicDeviceHolder& rMidiOpenedPorts, 
                                                                                          QObject*            parent) :
    QAbstractListModel(parent),
    m_rMusicDeviceHolder(rMidiOpenedPorts)
{
}

QHash<int, QByteArray> push2::qt::ControllerDeviceParameterCategoryModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}

int push2::qt::ControllerDeviceParameterCategoryModel::rowCount(const QModelIndex&) const
{
    if(UNSELECTED == m_currentDeviceIndex)
    {
        return 0;
    }
    return m_rMusicDeviceHolder.controllerDevice(m_currentDeviceIndex)->pDescr->controllerSection->parameter_categories.size();
}

QVariant push2::qt::ControllerDeviceParameterCategoryModel::data(const QModelIndex &index, int role) const
{
    const auto idx = index.row();
    switch(role)
    {
        case NameRole:
        {
            if(UNSELECTED != m_currentDeviceIndex)
            {
                return QString(m_rMusicDeviceHolder.controllerDevice(m_currentDeviceIndex)->pDescr->controllerSection->parameter_categories[idx].category_name.c_str());
            }
        }
        default:
        {
            break;
        }
    }
    return QVariant();
}

void push2::qt::ControllerDeviceParameterCategoryModel::onCurrentDeviceIndexChanged(int index)
{
    if(m_currentDeviceIndex != index)
    {
        QAbstractListModel::beginResetModel();
        m_currentDeviceIndex = index;
        QAbstractListModel::endResetModel();
    }
}