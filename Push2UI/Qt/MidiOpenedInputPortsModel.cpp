#include "MidiOpenedInputPortsModel.h"
#include <QUrl>

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
        return QString(m_rMidiOpenedPorts.controllerDevices.atIdx(idx).second->pDescr->productName.c_str());
    }
    return QString();
}

QUrl MidiOpenedInputPortsModel::getDevPicturePath(int idx) const noexcept
{
    if(idx < m_rMidiOpenedPorts.controllerDevices.size())
    {
        return QUrl(("qrc:///Artwork/Devices/" + m_rMidiOpenedPorts.controllerDevices.atIdx(idx).second->pDescr->configPath + "/Thumbnail.png").c_str());
    }
    return QUrl();
}
