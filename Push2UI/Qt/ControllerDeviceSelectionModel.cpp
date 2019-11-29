#include "ControllerDeviceSelectionModel.h"
#include <iostream>

using namespace push2::qt;

ControllerDeviceSelectionModel::ControllerDeviceSelectionModel(MusicDeviceHolder&   rMidiOpenedPorts,
                                                               QObject*             parent) :
    QAbstractListModel(parent),
    m_rMidiOpenedPorts(rMidiOpenedPorts)
{
}

QHash<int, QByteArray> ControllerDeviceSelectionModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole]    = "name";
    roles[PicturePathRole] = "picturepath";
    return roles;
}

int ControllerDeviceSelectionModel::rowCount(const QModelIndex&) const
{
    return m_rMidiOpenedPorts.controllerDevices.size();
}

QVariant ControllerDeviceSelectionModel::data(const QModelIndex &index, int role) const
{
    const auto idx = index.row();
    switch(role)
    {
        case NameRole:
        {
            if(idx >=0 && idx < m_rMidiOpenedPorts.controllerDevices.size())
            {
                return QString(m_rMidiOpenedPorts.controllerDevices.atIdx(idx).first.deviceName.c_str()).split(':').last();
            }
            break;
        }
        case PicturePathRole:
        {
            if(idx < m_rMidiOpenedPorts.controllerDevices.size())
            {
                return QString(m_rMidiOpenedPorts.controllerDevices.atIdx(idx).second->pDescr->thumbnail.c_str());
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return QVariant();
}