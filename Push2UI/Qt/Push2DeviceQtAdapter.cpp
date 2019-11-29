#include "Push2DeviceQtAdapter.h"
#include "Push2LedOfWidget.h"

push2::qt::Push2Device::Push2Device(push2device::Push2Device& rPush2Device, 
                                             QObject* parent):
   QObject(parent),
   m_rPush2Device(rPush2Device)
{
   for(int i = 0; i <= fp::Push2Topology::Button::eLast; ++i)
   {
      auto& thisRefBtn = static_cast<fp::Button::CallbackIf&>(*this);
      auto& thisRefLed = static_cast<fp::Led::ILedScene&>(*this);
      auto w = fp::Widget(i, fp::IdxAll, fp::IdxAll);
      m_rPush2Device.registerCB(thisRefBtn, w);
      m_rPush2Device.reserveLed(&thisRefLed, fp::getLedOfButton(w), {0, 0, 0});
   }
   {
      auto& thisRefBtn = static_cast<fp::Button3d::CallbackIf&>(*this);
      auto& thisRefLed = static_cast<fp::Led::ILedScene&>(*this);
      auto idBtn = uint8_t(fp::Push2Topology::Button3d::Id::eBtnSil);
      auto w = fp::Widget(idBtn, fp::IdxAll, fp::IdxAll);
      auto idLed = uint8_t(fp::Push2Topology::Led::Id::eBtnSilLed);
      m_rPush2Device.registerCB(thisRefBtn, w);
      m_rPush2Device.reserveLed(&thisRefLed, fp::getLedOfButton3d(w), {0, 0, 0});
   }
   for(int i = 0; i <= fp::Push2Topology::Encoder::eLast; ++i)
   {
      auto& thisRef = static_cast<fp::Encoder::CallbackIf&>(*this);
      auto fpWidget = fp::Widget(i, fp::IdxAll, fp::IdxAll);
      m_rPush2Device.registerCB(thisRef, fpWidget);
   }
   m_rPush2Device.registerCB(static_cast<fp::TouchSurface::CallbackIf&>(*this), fp::Widget(fp::Push2Topology::TouchSurface::eTouchStrip));
}

void push2::qt::Push2Device::registerCB(typename fp::Button::CallbackIf& cb, const fp::Widget& w)
{
   m_rPush2Device.registerCB(cb, w);
}

void push2::qt::Push2Device::registerCB(typename fp::Button3d::CallbackIf& cb, const fp::Widget& w)
{
   m_rPush2Device.registerCB(cb, w);
}

void push2::qt::Push2Device::registerCB(typename fp::Encoder::CallbackIf& cb, const fp::Widget& w)
{
   m_rPush2Device.registerCB(cb, w);
}

void push2::qt::Push2Device::registerCB(typename fp::TouchSurface::CallbackIf& cb, const fp::Widget& w)
{
   m_rPush2Device.registerCB(cb, w);
}

void push2::qt::Push2Device::unRegisterCB(typename fp::Button::CallbackIf& cb, const fp::Widget& w)
{
   m_rPush2Device.unRegisterCB(cb, w);
}

void push2::qt::Push2Device::unRegisterCB(typename fp::Button3d::CallbackIf& cb, const fp::Widget& w)
{
   m_rPush2Device.unRegisterCB(cb, w);
}

void push2::qt::Push2Device::unRegisterCB(typename fp::Encoder::CallbackIf& cb, const fp::Widget& w)
{
   m_rPush2Device.unRegisterCB(cb, w);
}

void push2::qt::Push2Device::unRegisterCB(typename fp::TouchSurface::CallbackIf& cb, const fp::Widget& w)
{
   m_rPush2Device.unRegisterCB(cb, w);
}

void push2::qt::Push2Device::reserveLed(fp::Led::ILedScene& ledScene, const QColor& color, const fp::Widget& w)
{
   m_rPush2Device.reserveLed(&ledScene, w, {uint32_t(color.red()), uint32_t(color.green()), uint32_t(color.blue())});
}

void push2::qt::Push2Device::reserveLedOfBtn(fp::Led::ILedScene& ledScene, const QColor& color, const fp::Widget& w)
{
   m_rPush2Device.reserveLed(&ledScene, getLedOfButton(w), {uint32_t(color.red()), uint32_t(color.green()), uint32_t(color.blue())});
}

void push2::qt::Push2Device::releaseLed(fp::Led::ILedScene& ledScene, const fp::Widget& w)
{
   m_rPush2Device.releaseLed(&ledScene, w);
}

void push2::qt::Push2Device::setLed(fp::Led::ILedScene& ledScene, const QColor& color, const fp::Widget& w)
{
   const auto ledColor = fp::Led::ColorRGB({uint32_t(color.red()), uint32_t(color.green()), uint32_t(color.blue())});
   m_rPush2Device.setLedOfScene(&ledScene, w, ledColor);
}

void push2::qt::Push2Device::setLedOfBtn(fp::Led::ILedScene& ledScene, const QColor& color, const fp::Widget& w)
{
   m_rPush2Device.setLedOfScene(&ledScene, getLedOfButton(w), {uint32_t(color.red()), uint32_t(color.green()), uint32_t(color.blue())});
}

void push2::qt::Push2Device::onPressStateChange(const fp::Button::PressState& pressState, const fp::Widget& w)
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

void push2::qt::Push2Device::onPressStateChange(const fp::Button3d::StateData& data, const Widget& w)
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

void push2::qt::Push2Device::onPositionEvents(const uint32_t& pressure, const Widget& w)
{
   // TODO ?
}

void push2::qt::Push2Device::onIncrement(int32_t data, const fp::Widget& w)
{
   emit encoderIncrementChanged(data, static_cast<Encoder::Id>(w.id), w.coord.x, w.coord.y);
}

void push2::qt::Push2Device::onTouchStateChanged(fp::Encoder::TouchState touchState, const fp::Widget& w)
{
   emit encoderTouchStateChanged(static_cast<EncoderTouchState::Value>(touchState), static_cast<Encoder::Id>(w.id), w.coord.x, w.coord.y);
}

void push2::qt::Push2Device::onTouchStateChanged(fp::TouchSurface::TouchState touchState, const Widget& w)
{
   emit touchSurfaceTouchStateChanged(static_cast<TouchSurfaceTouchState::Value>(touchState), static_cast<TouchSurface::Id>(w.id), w.coord.x, w.coord.y);
}

void push2::qt::Push2Device::onPositionEvents(const fp::TouchSurface::PressData& data, const Widget& w)
{
   emit touchSurfaceTouchPositionChanged(data.pos.x, data.pos.y, data.pressure, static_cast<TouchSurface::Id>(w.id), w.coord.x, w.coord.y);
}

void push2::qt::Push2Device::onReserveLed(const QColor& color, Led::Id id, int x, int y)
{
   m_rPush2Device.reserveLed(this, fp::Widget(id, x, y), {uint32_t(color.red()), uint32_t(color.green()), uint32_t(color.blue())});
}

void push2::qt::Push2Device::onReserveLedOfBtn(const QColor& color, Button::Id id, int x, int y)
{
   m_rPush2Device.reserveLed(this, getLedOfButton(fp::Widget(id, x, y)), {uint32_t(color.red()), uint32_t(color.green()), uint32_t(color.blue())});
}

void push2::qt::Push2Device::onSetLed(const QColor& color, Led::Id id, int x, int y)
{
   m_rPush2Device.setLedOfScene(this, fp::Widget(id, x, y), {uint32_t(color.red()), uint32_t(color.green()), uint32_t(color.blue())});
}

void push2::qt::Push2Device::onSetLedOfBtn(const QColor& color, Button::Id id, int x, int y)
{
   m_rPush2Device.setLedOfScene(this, getLedOfButton(fp::Widget(id, x, y)), {uint32_t(color.red()), uint32_t(color.green()), uint32_t(color.blue())});
}

push2::qt::ButtonPressState::Value push2::qt::Push2Device::getButtonState(Button::Id id, int x, int y) const
{
   return static_cast<ButtonPressState::Value>(m_rPush2Device.getButtonState(fp::Widget(id, x, y)));
}
