#include "Push2DeviceOverlay.h"
#include <QCoreApplication>
#include "Push2LedOfWidget.h"

push2::qt::Push2DeviceOverlay::Push2DeviceOverlay()
{
    m_pPush2Device = QCoreApplication::instance()->findChild<push2::qt::Push2Device*>("push2DeviceQtAdapter");
}

push2::qt::Push2DeviceOverlay::~Push2DeviceOverlay()
{
    for(auto &w : m_reservedButtons) m_pPush2Device->unRegisterCB(static_cast<fp::Button::CallbackIf&>(*this), w);
    for(auto &w : m_reservedButton3ds) m_pPush2Device->unRegisterCB(static_cast<fp::Button3d::CallbackIf&>(*this), w);
    for(auto &w : m_reservedEncoders) m_pPush2Device->unRegisterCB(static_cast<fp::Encoder::CallbackIf&>(*this), w);
    for(auto &w : m_reservedTouchSurfaces) m_pPush2Device->unRegisterCB(static_cast<fp::TouchSurface::CallbackIf&>(*this), w);
    for(auto &w : m_reservedLeds) m_pPush2Device->releaseLed(*this, w);
}

push2::qt::ButtonPressState::Value push2::qt::Push2DeviceOverlay::getButtonState(Button::Id id, int x, int y) const
{
    return m_pPush2Device->getButtonState(id, x, y);
}

void push2::qt::Push2DeviceOverlay::onReserveButton(Button::Id id, int x, int y)
{
    const auto w = fp::Widget(id, x, y);
    m_pPush2Device->registerCB(static_cast<fp::Button::CallbackIf&>(*this), w);
    m_reservedButtons.insert(w);
}

void push2::qt::Push2DeviceOverlay::onReserveButton3d(Button3d::Id id, int x, int y)
{
    const auto w = fp::Widget(id, x, y);
    m_pPush2Device->registerCB(static_cast<fp::Button3d::CallbackIf&>(*this), w);
    m_reservedButton3ds.insert(w);
}

void push2::qt::Push2DeviceOverlay::onReserveEncoder(Encoder::Id id, int x, int y)
{
    const auto w = fp::Widget(id, x, y);
    m_pPush2Device->registerCB(static_cast<fp::Encoder::CallbackIf&>(*this), w);
    m_reservedEncoders.insert(w);
}

void push2::qt::Push2DeviceOverlay::onReserveTouchSurface(TouchSurface::Id id, int x, int y)
{
    const auto w = fp::Widget(id, x, y);
    m_pPush2Device->registerCB(static_cast<fp::TouchSurface::CallbackIf&>(*this), w);
    m_reservedTouchSurfaces.insert(w);
}

void push2::qt::Push2DeviceOverlay::onReserveLed(const QColor& color, Led::Id id, int x, int y)
{
    const auto w = fp::Widget(id, x, y);
    m_pPush2Device->reserveLed(static_cast<fp::Led::ILedScene&>(*this), color, w);
    m_reservedLeds.insert(w);
}

void push2::qt::Push2DeviceOverlay::onReserveLedOfBtn(const QColor& color, Button::Id id, int x, int y)
{
    const auto w = fp::getLedOfButton(fp::Widget(id, x, y));
    m_pPush2Device->reserveLed(static_cast<fp::Led::ILedScene&>(*this), color, w);
    m_reservedLeds.insert(w);
}

void push2::qt::Push2DeviceOverlay::onReleaseButton(Button::Id id, int x, int y)
{
    const auto w = fp::Widget(id, x, y);
    m_pPush2Device->unRegisterCB(static_cast<fp::Button::CallbackIf&>(*this), w);
    m_reservedButtons.erase(w);
}

void push2::qt::Push2DeviceOverlay::onReleaseButton3d(Button3d::Id id, int x, int y)
{
    const auto w = fp::Widget(id, x, y);
    m_pPush2Device->unRegisterCB(static_cast<fp::Button3d::CallbackIf&>(*this), w);
    m_reservedButton3ds.erase(w);
}

void push2::qt::Push2DeviceOverlay::onReleaseEncoder(Encoder::Id id, int x, int y)
{
    const auto w = fp::Widget(id, x, y);
    m_pPush2Device->unRegisterCB(static_cast<fp::Encoder::CallbackIf&>(*this), w);
    m_reservedEncoders.erase(w);
}

void push2::qt::Push2DeviceOverlay::onReleaseTouchSurface(TouchSurface::Id id, int x, int y)
{
    const auto w = fp::Widget(id, x, y);
    m_pPush2Device->unRegisterCB(static_cast<fp::TouchSurface::CallbackIf&>(*this), w);
    m_reservedTouchSurfaces.erase(w);
}

void push2::qt::Push2DeviceOverlay::onReleaseLed(Led::Id id, int x, int y)
{
    const auto w = fp::Widget(id, x, y);
    m_pPush2Device->releaseLed(static_cast<fp::Led::ILedScene&>(*this), w);
    m_reservedLeds.erase(w);
}

void push2::qt::Push2DeviceOverlay::onReleaseLedOfBtn(Button::Id id, int x, int y)
{
    const auto w = fp::getLedOfButton(fp::Widget(id, x, y));
    m_pPush2Device->releaseLed(static_cast<fp::Led::ILedScene&>(*this), w);
    m_reservedLeds.erase(w);
}

void push2::qt::Push2DeviceOverlay::onSetLed(const QColor& color, Led::Id id, int x, int y)
{
    const auto w = fp::Widget(id, x, y);
    m_pPush2Device->setLed(static_cast<fp::Led::ILedScene&>(*this), color, w);
}

void push2::qt::Push2DeviceOverlay::onSetLedOfBtn(const QColor& color, Button::Id id, int x, int y)
{
    const auto w = fp::Widget(id, x, y);
    m_pPush2Device->setLedOfBtn(static_cast<fp::Led::ILedScene&>(*this), color, w);
}

void push2::qt::Push2DeviceOverlay::onPressStateChange(const fp::Button::PressState& pressState, const fp::Widget& w)
{
   emit buttonPressStateChange(static_cast<ButtonPressState::Value>(pressState), static_cast<Button::Id>(w.id), w.coord.x, w.coord.y);
   switch(pressState)
   {
      case fp::Button::PressState::Pressed:
      {
         emit buttonPressed(static_cast<Button::Id>(w.id), w.coord.x, w.coord.y);
         break;
      }
      case fp::Button::PressState::Released:
      {
         emit buttonReleased(static_cast<Button::Id>(w.id), w.coord.x, w.coord.y);
         break;
      }
      default: break;
   }
}

void push2::qt::Push2DeviceOverlay::onPressStateChange(const fp::Button3d::StateData& data, const Widget& w)
{
   emit button3dPressStateChange(static_cast<Button3dPressState::Value>(data.pressState), data.velocity, static_cast<Button3d::Id>(w.id), w.coord.x, w.coord.y);
   switch(data.pressState)
   {
      case fp::Button3d::PressState::Pressed:
      {
         emit button3dPressed(data.velocity, static_cast<Button3d::Id>(w.id), w.coord.x, w.coord.y);
         break;
      }
      case fp::Button3d::PressState::Released:
      {
         emit button3dReleased(data.velocity, static_cast<Button3d::Id>(w.id), w.coord.x, w.coord.y);
         break;
      }
      default: break;
   }
}

void push2::qt::Push2DeviceOverlay::onPositionEvents(const uint32_t& pressure, const Widget& w)
{
   // TODO ?
}

void push2::qt::Push2DeviceOverlay::onIncrement(int32_t data, const fp::Widget& w)
{
   emit encoderIncrementChanged(data, static_cast<Encoder::Id>(w.id), w.coord.x, w.coord.y);
}

void push2::qt::Push2DeviceOverlay::onTouchStateChanged(fp::Encoder::TouchState touchState, const fp::Widget& w)
{
   emit encoderTouchStateChanged(static_cast<EncoderTouchState::Value>(touchState), static_cast<Encoder::Id>(w.id), w.coord.x, w.coord.y);
}

void push2::qt::Push2DeviceOverlay::onTouchStateChanged(fp::TouchSurface::TouchState touchState, const Widget& w)
{
   emit touchSurfaceTouchStateChanged(static_cast<TouchSurfaceTouchState::Value>(touchState), static_cast<TouchSurface::Id>(w.id), w.coord.x, w.coord.y);
}

void push2::qt::Push2DeviceOverlay::onPositionEvents(const fp::TouchSurface::PressData& data, const Widget& w)
{
   emit touchSurfaceTouchPositionChanged(data.pos.x, data.pos.y, data.pressure, static_cast<TouchSurface::Id>(w.id), w.coord.x, w.coord.y);
}
