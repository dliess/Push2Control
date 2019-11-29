#ifndef PUSH2_QT_STARTABLE_DEVICES_MODEL_H
#define PUSH2_QT_STARTABLE_DEVICES_MODEL_H

#include "MusicDeviceHolder.h"
#include <QObject>
#include <QAbstractListModel>
#include <QHash>

class TransportControl;

namespace push2::qt
{

class StartableDevicesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        PicturePathRole,
        EnabledRole
    };
    StartableDevicesModel(MusicDeviceHolder::MusicDevices& rMusicDevices,
                          TransportControl&                rTransportControl) noexcept;
private:
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    MusicDeviceHolder::MusicDevices& m_rMusicDevices;
    TransportControl&                m_rTransportControl;
};

} // namespace push2::qt

#endif