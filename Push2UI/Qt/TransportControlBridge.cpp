#include "TransportControlBridge.h"
#include "TransportControl.h"


push2::qt::TransportControlBridge::TransportControlBridge(MusicDeviceHolder::MusicDevices& rMusicDevices,
                                                          TransportControl& rTransportControl) noexcept:
   QObject(),
   m_rTransportControl(rTransportControl),
   m_startableDevicesModel(rMusicDevices, rTransportControl)
{
   m_rTransportControl.registerStartedChangeNotifCb([this](bool){
      emit startedChanged();
   });
}

void push2::qt::TransportControlBridge::toggleStartAllEnabled() noexcept
{
   if(m_rTransportControl.getStarted())
   {
      m_rTransportControl.stopAllEnabled();
   }
   else
   {
      m_rTransportControl.startAllEnabled();
   }
}

bool push2::qt::TransportControlBridge::started() const noexcept
{
   return m_rTransportControl.getStarted();
}

void push2::qt::TransportControlBridge::toggleEnabled(int musicDeviceIndex) noexcept
{
   m_rTransportControl.toggleEnabled(musicDeviceIndex);
}

QAbstractListModel* push2::qt::TransportControlBridge::startableDevicesModel()
{
   return &m_startableDevicesModel;
}