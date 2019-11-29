#include "MidiOpenedInputPortsModel.h"

using namespace push2::qt;


MidiOpenedInputPortsModel::MidiOpenedInputPortsModel(MusicDeviceHolder& rMidiOpenedPorts, 
                                                     QObject *parent) :
QAbstractListModel(parent),
m_rMidiOpenedPorts(rMidiOpenedPorts)
{
}

QHash<int, QByteArray> MidiOpenedInputPortsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[PicturePath] = "picturepath";
    return roles;
}

int MidiOpenedInputPortsModel::rowCount(const QModelIndex&) const
{
    return m_rMidiOpenedPorts.controllerDevices.size();
}

QVariant MidiOpenedInputPortsModel::data(const QModelIndex &index, int role) const
{
    const auto idx = index.row();
    if(idx < 0 || idx >= rowCount(QModelIndex()))
    {
        return QVariant();
    }
    switch(role)
    {
        case NameRole:
        {
            return getDevName(idx);
        }
        case PicturePath:
        {
            return getDevPicturePath(idx);
        }
        default:
        {
            break;
        }
    }
    return QVariant();
}

QString MidiOpenedInputPortsModel::getDevName(int idx) const noexcept
{
    if(idx < m_rMidiOpenedPorts.controllerDevices.size())
    {
        return QString(m_rMidiOpenedPorts.controllerDevices.atIdx(idx).second->pDescr->product_name.c_str());
    }
    return QString();
}

QString MidiOpenedInputPortsModel::getDevPicturePath(int idx) const noexcept
{
    if(idx < m_rMidiOpenedPorts.controllerDevices.size())
    {
        return QString(m_rMidiOpenedPorts.controllerDevices.atIdx(idx).second->pDescr->thumbnail.c_str());
    }
    return QString();
}
