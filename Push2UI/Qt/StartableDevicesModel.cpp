#include "StartableDevicesModel.h"
#include "TransportControl.h"

push2::qt::StartableDevicesModel::StartableDevicesModel(MusicDeviceHolder::MusicDevices& rMusicDevices,
                                                        TransportControl&                rTransportControl) noexcept :
    m_rMusicDevices(rMusicDevices),
    m_rTransportControl(rTransportControl)
{
    m_rTransportControl.registerEnableMaskChangeNotifCb([this](int musicDeviceIndex){
        emit dataChanged(createIndex(musicDeviceIndex, 0),
                         createIndex(musicDeviceIndex , 0),
                         { EnabledRole } );
    });
}

QHash<int, QByteArray> push2::qt::StartableDevicesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole]        = "name";
    roles[PicturePathRole] = "picturepath";
    roles[EnabledRole]     = "deviceenabled";
    return roles;
}

int push2::qt::StartableDevicesModel::rowCount(const QModelIndex&) const
{
    return m_rMusicDevices.size();
}

QVariant push2::qt::StartableDevicesModel::data(const QModelIndex &index, int role) const
{
    const auto idx = index.row();
    switch(role)
    {
        case NameRole:
        {
            return QString(m_rMusicDevices.atIdx(idx).first.deviceName.c_str()); //.split(':').last();
        }
        case PicturePathRole:
        {
            return QString(m_rMusicDevices.atIdx(idx).second->pDescr->thumbnail.c_str());
        }
        case EnabledRole:
        {
            return m_rTransportControl.getIfEnabled(idx);
        }
        default: break;
    }
    return QVariant();
}
