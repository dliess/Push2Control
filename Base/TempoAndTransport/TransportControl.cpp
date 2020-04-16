#include "TransportControl.h"
#include <loguru.hpp>

TransportControl::TransportControl(MusicDeviceHolder::MusicDevices& musicDevices) noexcept:
   m_rMusicDevices(musicDevices)
{}

void TransportControl::startAllEnabled() noexcept
{
   if(m_started) return;
   for(int i = 0; i < m_rMusicDevices.size(); ++i)
   {
      if(m_enableMask & (1 << i))
      {
         m_rMusicDevices.atIdx(i).second->startSequencer();
      }
   }
   m_started = true;
   if(m_startedChangeNotifCb) m_startedChangeNotifCb(m_started);
}

void TransportControl::stopAllEnabled() noexcept
{
   if(!m_started) return;
   for(int i = 0; i < m_rMusicDevices.size(); ++i)
   {
      if(m_enableMask & (1 << i))
      {
         m_rMusicDevices.atIdx(i).second->stopSequencer();
      }
   }
   m_started = false;
   if(m_startedChangeNotifCb) m_startedChangeNotifCb(m_started);
}

bool TransportControl::getStarted() const noexcept
{
   return m_started;
}

void TransportControl::toggleEnabled(int musicDeviceIndex) noexcept
{
   if(musicDeviceIndex < 0 || musicDeviceIndex >= m_rMusicDevices.size())
   {
      LOG_F(ERROR, "invalid index {}", musicDeviceIndex);
      return;
   }
   m_enableMask ^= (1 << musicDeviceIndex);
   if(m_enableMask & (1 << musicDeviceIndex))
   {
      if(m_started)
      {
         m_rMusicDevices.atIdx(musicDeviceIndex).second->startSequencer();
      }
   }
   else
   {
      m_rMusicDevices.atIdx(musicDeviceIndex).second->stopSequencer();
   }
   if(m_enableMaskChangeNotifCb) m_enableMaskChangeNotifCb(musicDeviceIndex);
}

bool TransportControl::getIfEnabled(int musicDeviceIndex) const noexcept
{
   return m_enableMask & (1 << musicDeviceIndex);
}

void TransportControl::registerStartedChangeNotifCb(std::function<void(bool)> cb) noexcept
{
   m_startedChangeNotifCb = cb;
}

void TransportControl::registerEnableMaskChangeNotifCb(std::function<void(int)> cb) noexcept
{
   m_enableMaskChangeNotifCb = cb;
}

TransportControl::Settings TransportControl::getSettings() const noexcept
{
   Settings settings;
   for(int i = 0; i < m_rMusicDevices.size(); ++i)
   {
      if(!getIfEnabled(i))
      {
         settings.push_back(m_rMusicDevices.atIdx(i).first.toStr());
      }
   }
   return settings;
}

void TransportControl::setSettings(const Settings& settings) noexcept
{
   for(auto &disabledMusicDeviceId : settings)
   {
      const auto idx = m_rMusicDevices.getIndexOf(disabledMusicDeviceId);
      if(getIfEnabled(idx))
      {
         toggleEnabled(idx);
      }
   }
}
