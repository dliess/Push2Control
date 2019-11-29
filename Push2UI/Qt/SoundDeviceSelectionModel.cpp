#include "SoundDeviceSelectionModel.h"
#include <iostream>

using namespace push2::qt;

SoundDeviceSelectionModel::SoundDeviceSelectionModel(MusicDeviceHolder&   rMidiOpenedPorts,
                                                     QObject*             parent) :
    QAbstractListModel(parent),
    m_rMidiOpenedPorts(rMidiOpenedPorts)
{
}

QHash<int, QByteArray> SoundDeviceSelectionModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole]    = "name";
    roles[PicturePathRole] = "picturepath";
    return roles;
}

int SoundDeviceSelectionModel::rowCount(const QModelIndex&) const
{
    return m_rMidiOpenedPorts.soundDevices.size();
}

QVariant SoundDeviceSelectionModel::data(const QModelIndex &index, int role) const
{
    const auto idx = index.row();
    switch(role)
    {
        case NameRole:
        {
            if(idx >=0 && idx < m_rMidiOpenedPorts.soundDevices.size())
            {
                return QString(m_rMidiOpenedPorts.soundDevices.atIdx(idx).first.deviceName.c_str()).split(':').last();
            }
            break;
        }
        case PicturePathRole:
        {
            if(idx < m_rMidiOpenedPorts.soundDevices.size())
            {
                return QString(m_rMidiOpenedPorts.soundDevices.atIdx(idx).second->pDescr->thumbnail.c_str());
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