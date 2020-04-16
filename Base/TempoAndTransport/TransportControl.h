#ifndef TRANSPORT_CONTROL_H
#define TRANSPORT_CONTROL_H

#include "MusicDeviceHolder.h"
#include "Settings.h"
#include <functional>
#include <vector>
#include <string>

class TransportControl : public utils::Settings<TransportControl>
{
public:
   TransportControl(MusicDeviceHolder::MusicDevices& musicDevices) noexcept;
   void startAllEnabled() noexcept;
   void stopAllEnabled() noexcept;
   bool getStarted() const noexcept;
   void toggleEnabled(int musicDeviceIndex) noexcept;
   bool getIfEnabled(int musicDeviceIndex) const noexcept;
   void registerStartedChangeNotifCb(std::function<void(bool)> cb) noexcept;
   void registerEnableMaskChangeNotifCb(std::function<void(int)> cb) noexcept;
   // ============== Settings ===============
   using Settings = std::vector<std::string>;
   Settings getSettings() const noexcept;
   void setSettings(const Settings& settings) noexcept;
   // =======================================
private:
   MusicDeviceHolder::MusicDevices& m_rMusicDevices;
   bool                             m_started{false};
   uint32_t                         m_enableMask{0xFFFFFFFF};
   std::function<void(bool)>        m_startedChangeNotifCb;
   std::function<void(int)>         m_enableMaskChangeNotifCb;
};

#endif