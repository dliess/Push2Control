#ifndef TEMPO_HANDLER_BRIDGE_H
#define TEMPO_HANDLER_BRIDGE_H

#include <QObject>

class TempoHandler;

namespace push2::qt
{

class TempoHandlerBridge : public QObject
{
   Q_OBJECT
   Q_PROPERTY(qreal bpm READ bpm NOTIFY bpmChanged)
public:
   TempoHandlerBridge(TempoHandler& rTempoHandler) noexcept;
   qreal bpm() const noexcept;
   Q_INVOKABLE void incBpm(qreal bpmIncr) noexcept;
   Q_INVOKABLE void setNudgeBpm(qreal nudgeBpm) noexcept;

signals:
   void bpmChanged();
private:
   TempoHandler& m_rTempoHandler;
};

} // namespace push2::qt

#endif