#ifndef TRANSPORT_CONTROL_BRIDGE_H
#define TRANSPORT_CONTROL_BRIDGE_H

#include "StartableDevicesModel.h"
#include "MusicDeviceHolder.h"
#include <QObject>

class TransportControl;

namespace push2::qt
{

class TransportControlBridge : public QObject
{
   Q_OBJECT
   Q_PROPERTY(bool started READ started NOTIFY startedChanged)
   Q_PROPERTY(QAbstractListModel* startableDevicesModel READ startableDevicesModel NOTIFY startableDevicesModelChanged)
public:
   TransportControlBridge(MusicDeviceHolder::MusicDevices& rMusicDevices,
                          TransportControl&                rTransportControl) noexcept;
   Q_INVOKABLE void toggleStartAllEnabled() noexcept;
   bool started() const noexcept;
   Q_INVOKABLE void toggleEnabled(int musicDeviceIndex) noexcept;
   QAbstractListModel* startableDevicesModel();

signals:
   void startedChanged();
   void startableDevicesModelChanged(); // DUMMY, makes QML happy
private:
   TransportControl&     m_rTransportControl;
   StartableDevicesModel m_startableDevicesModel;
};


} // namespace push2::qt

#endif