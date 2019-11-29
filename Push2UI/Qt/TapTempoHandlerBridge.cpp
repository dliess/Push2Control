#include "TapTempoHandlerBridge.h"
#include "TapTempoHandler.h"

push2::qt::TapTempoHandlerBridge::TapTempoHandlerBridge(TapTempoHandler& rTapTempoHandler) noexcept :
    QObject(),
    m_rTapTempoHandler(rTapTempoHandler)
{}

void push2::qt::TapTempoHandlerBridge::tap() noexcept
{
    m_rTapTempoHandler.tap();
}