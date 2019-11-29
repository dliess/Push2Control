#ifndef TAP_TEMPO_HANDLER_BRIDGE_H
#define TAP_TEMPO_HANDLER_BRIDGE_H

#include <QObject>

class TapTempoHandler;

namespace push2::qt
{

class TapTempoHandlerBridge : public QObject
{
    Q_OBJECT
public:
    TapTempoHandlerBridge(TapTempoHandler& rTapTempoHandler) noexcept;
    Q_INVOKABLE void tap() noexcept;
private:
    TapTempoHandler& m_rTapTempoHandler;
};

} // namespace push2::qt
#endif