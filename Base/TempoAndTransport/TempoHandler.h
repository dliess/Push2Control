#ifndef TEMPO_HANDLER_H
#define TEMPO_HANDLER_H

#include "MusicDeviceHolder.h"
#include <chrono>
#include <functional>
#include <atomic>
#include <cstdint>

class TempoHandler
{
public:
   TempoHandler(MusicDeviceHolder::MusicDevices& musicDevices) noexcept;
   void start() noexcept;
   void stop() noexcept;
   void nextTimeSlot() noexcept;
   void incBpm(int cents) noexcept;
   void setBpmCents(int cents) noexcept;
   void setNudgeCents(int cents) noexcept;
   int getBpmCents() const noexcept;
   void registerBpmChangeNotifCb(std::function<void(int)> cb) noexcept;
   std::chrono::nanoseconds getBeatPeriodNs() const noexcept;
private:
   std::chrono::nanoseconds calcPeriodNs() const noexcept;
   void notifyMusicDevices() noexcept;
   MusicDeviceHolder::MusicDevices&      m_rMusicDevices;
   std::atomic<bool>                     m_running{false};
   std::atomic<int>                      m_bpmCents{12000};
   std::atomic<int>                      m_nudgeCents{0};
   uint32_t                              m_notificationMask{0xFFFFFFFF};
   std::chrono::system_clock::time_point m_nextNotificationTimePoint;
   std::function<void(int)>              m_bpmChangeNotifCb;
   static constexpr int PPQ = 24;
};

#endif