#include "TempoHandler.h"
#include <loguru.hpp>

TempoHandler::TempoHandler(MusicDeviceHolder::MusicDevices& musicDevices) noexcept:
   m_rMusicDevices(musicDevices),
   m_nextNotificationTimePoint(std::chrono::high_resolution_clock::now())
{}

void TempoHandler::start() noexcept
{
   if(m_running) return;
   m_nextNotificationTimePoint = std::chrono::high_resolution_clock::now();
   m_running = true;
}

void TempoHandler::stop() noexcept
{
   m_running = false;
}

void TempoHandler::nextTimeSlot() noexcept
{
   if(!m_running) return;
   if(std::chrono::high_resolution_clock::now() >= m_nextNotificationTimePoint)
   {
     // LOG_F(INFO, "period {} ns", calcPeriodNs().count());
      m_nextNotificationTimePoint += calcPeriodNs();
      notifyMusicDevices();
   }
}

void TempoHandler::incBpm(int cents) noexcept
{
   if(m_bpmCents + cents < 2 || m_bpmCents + cents > 40000) return;
   m_bpmCents.fetch_add(cents);
   if(m_bpmChangeNotifCb) m_bpmChangeNotifCb(getBpmCents());
}

void TempoHandler::setBpmCents(int cents) noexcept
{
   if(cents <= 0 || m_bpmCents == cents) return;
   m_bpmCents = cents;
   if(m_bpmChangeNotifCb) m_bpmChangeNotifCb(getBpmCents());
}

void TempoHandler::setNudgeCents(int cents) noexcept
{
   m_nudgeCents = cents;
   if(m_bpmChangeNotifCb) m_bpmChangeNotifCb(getBpmCents());
}

int TempoHandler::getBpmCents() const noexcept
{
   return m_bpmCents + m_nudgeCents;
}

void TempoHandler::registerBpmChangeNotifCb(std::function<void(int)> cb) noexcept
{
   m_bpmChangeNotifCb = cb;
}

std::chrono::nanoseconds TempoHandler::getBeatPeriodNs() const noexcept
{
   return calcPeriodNs() * PPQ;
}

std::chrono::nanoseconds TempoHandler::calcPeriodNs() const noexcept
{
   constexpr uint64_t NSEC_PER_MIN = std::chrono::nanoseconds(std::chrono::minutes(1)).count();
   return std::chrono::duration<int64_t, std::nano>( (NSEC_PER_MIN * 100) / (getBpmCents() * PPQ) );
}

void TempoHandler::notifyMusicDevices() noexcept
{
   for(int i = 0; i < m_rMusicDevices.size(); ++i)
   {
      if(m_notificationMask & (1 << i))
      {
         m_rMusicDevices.atIdx(i).second->quarterNotePer24Tick();
      }
   }
}
