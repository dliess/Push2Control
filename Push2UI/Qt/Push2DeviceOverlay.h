#ifndef QT_ADAPTER_PUSH2_DEVICE_OVERLAY_H
#define QT_ADAPTER_PUSH2_DEVICE_OVERLAY_H

#include <QObject>
#include "Push2DeviceQtAdapter.h"
#include "FpButton.h"
#include "FpButton3d.h"
#include "FpEncoder.h"
#include "FpTouchSurface.h"
#include "FpLed.h"
#include <set>

namespace push2::qt
{

class Push2DeviceOverlay : public QObject,
                           public fp::Button::CallbackIf,
                           public fp::Button3d::CallbackIf,
                           public fp::Encoder::CallbackIf,
                           public fp::TouchSurface::CallbackIf,
                           public fp::Led::ILedScene
{
    Q_OBJECT
public:
   explicit Push2DeviceOverlay();
   ~Push2DeviceOverlay();
   Q_INVOKABLE ButtonPressState::Value getButtonState(Button::Id id, int x = 0, int y = 0) const;
public slots:
   void onReserveButton(Button::Id id, int x = 0, int y = 0);
   void onReserveButton3d(Button3d::Id id, int x = 0, int y = 0);
   void onReserveEncoder(Encoder::Id id, int x = 0, int y = 0);
   void onReserveTouchSurface(TouchSurface::Id id, int x = 0, int y = 0);
   void onReserveLed(const QColor& color, Led::Id id, int x = 0, int y = 0);
   void onReserveLedOfBtn(const QColor& color, Button::Id id, int x = 0, int y = 0);

   void onReleaseButton(Button::Id id, int x = 0, int y = 0);
   void onReleaseButton3d(Button3d::Id id, int x = 0, int y = 0);
   void onReleaseEncoder(Encoder::Id id, int x = 0, int y = 0);
   void onReleaseTouchSurface(TouchSurface::Id id, int x = 0, int y = 0);
   void onReleaseLed(Led::Id id, int x = 0, int y = 0);
   void onReleaseLedOfBtn(Button::Id id, int x = 0, int y = 0);

   void onSetLed(const QColor& color, Led::Id id, int x = 0, int y = 0);
   void onSetLedOfBtn(const QColor& color, Button::Id id, int x = 0, int y = 0);

private:
   push2::qt::Push2Device* m_pPush2Device{nullptr};
   void onPressStateChange(const fp::Button::PressState& pressState, const fp::Widget& w) final;
   void onPressStateChange(const fp::Button3d::StateData& data, const Widget& w) final;
   void onPositionEvents(const uint32_t& pressure, const Widget& w) final;
   void onIncrement(int32_t data, const fp::Widget& w) final;
   void onTouchStateChanged(fp::Encoder::TouchState touchState, const fp::Widget& w) final;
   void onTouchStateChanged(fp::TouchSurface::TouchState touchState, const Widget& w) final;
   void onPositionEvents(const fp::TouchSurface::PressData& data, const Widget& w) final;
   void onGotRevealed(const fp::Widget&) final {};
   void onGotHidden(const fp::Widget&) final {};

   std::set<fp::Widget> m_reservedButtons;
   std::set<fp::Widget> m_reservedButton3ds;
   std::set<fp::Widget> m_reservedEncoders;
   std::set<fp::Widget> m_reservedTouchSurfaces;
   std::set<fp::Widget> m_reservedLeds;

signals:
   void buttonPressStateChange(ButtonPressState::Value pressState, Button::Id id, int x, int y);
   void button3dPressStateChange(Button3dPressState::Value pressState, int velocity, Button3d::Id id, int x, int y); // TODO: "velocity" maybe float?
   void buttonPressed(Button::Id id, int x, int y);
   void buttonReleased(Button::Id id, int x, int y);
   void button3dPressed(int velocity, Button3d::Id id, int x, int y); // TODO: "velocity" maybe float?
   void button3dReleased(int velocity, Button3d::Id id, int x, int y); // TODO: "velocity" maybe float?
   void encoderIncrementChanged(int increment, Encoder::Id id, int x, int y);
   void encoderTouchStateChanged(EncoderTouchState::Value state, Encoder::Id id, int x, int y);
   void touchSurfaceTouchStateChanged(TouchSurfaceTouchState::Value state, TouchSurface::Id id, int x, int y);
   void touchSurfaceTouchPositionChanged(int posX, int posY, int pressure, TouchSurface::Id id, int x, int y); // TODO: maybe float?
};

} // namespace push2::qt

#endif