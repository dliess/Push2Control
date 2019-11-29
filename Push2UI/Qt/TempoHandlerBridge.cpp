#include "TempoHandlerBridge.h"
#include "TempoHandler.h"


push2::qt::TempoHandlerBridge::TempoHandlerBridge(TempoHandler& rTempoHandler) noexcept:
   QObject(),
   m_rTempoHandler(rTempoHandler)
{
   m_rTempoHandler.registerBpmChangeNotifCb([this](int newBpm){
      emit bpmChanged();
   });
}

qreal push2::qt::TempoHandlerBridge::bpm() const noexcept
{
   return m_rTempoHandler.getBpmCents() / 100.0;
}

void push2::qt::TempoHandlerBridge::incBpm(qreal bpmIncr) noexcept
{
   m_rTempoHandler.incBpm(bpmIncr * 100);
}

void push2::qt::TempoHandlerBridge::setNudgeBpm(qreal nudgeBpm) noexcept
{
   m_rTempoHandler.setNudgeCents(nudgeBpm * 100);
}