#ifndef QT_ADAPTER_PUSH2_DEVICE_H
#define QT_ADAPTER_PUSH2_DEVICE_H

#include <QObject>
#include <QColor>
#include "Push2Device.h"

namespace push2::qt
{

// --------------------------------------------------------
// ------ FpBase adapters ---------------------------------
// --------------------------------------------------------
namespace FpWidget
{
Q_NAMESPACE
enum class Idx {
   E_All = fp::IdxAll
};
Q_ENUM_NS(Idx)
} // namespace FpWidget

namespace ButtonPressState
{
Q_NAMESPACE
enum class Value {
   Pressed = fp::Button::PressState::Pressed,
   Released = fp::Button::PressState::Released
};
Q_ENUM_NS(Value)
} // namespace ButtonPressState

namespace Button3dPressState
{
Q_NAMESPACE
enum class Value {
   Pressed = fp::Button3d::PressState::Pressed,
   Released = fp::Button3d::PressState::Released
};
Q_ENUM_NS(Value)
} // namespace Button3dPressState

namespace EncoderTouchState
{
Q_NAMESPACE
enum class Value {
   Released = fp::Encoder::TouchState::Released,
   Touched = fp::Encoder::TouchState::Touched
};
Q_ENUM_NS(Value)
} // namespace EncoderTouchState

namespace TouchSurfaceTouchState
{
Q_NAMESPACE
enum class Value {
   Released = fp::TouchSurface::TouchState::Released,
   Touched = fp::TouchSurface::TouchState::Touched
};
Q_ENUM_NS(Value)
} // namespace TouchSurfaceTouchState

// --------------------------------------------------------
// ------ Push2 Fp Topology adapters ----------------------
// --------------------------------------------------------

namespace Led
{
   Q_NAMESPACE
   enum Id
   {
      E_LedAddTrack = 0,
      E_LedAccent = 1,
      E_LedFixedLength = 2,
      E_LedAddDevice = 3,
      E_LedSession = 4,
      E_LedSetup = 5,
      E_LedMute = 6,
      E_LedLeftArrow = 7,
      E_LedConvert = 8,
      E_LedStopClip = 9,
      E_LedBrowse = 10,
      E_LedRepeat = 11,
      E_LedSolo = 12,
      E_LedSel = 13,
      E_LedShift = 14,
      E_BtnSilLed = 15,
      E_LedScale = 16,
      E_LedPlay = 17,
      E_LedDelete = 18,
      E_LedNote = 19,
      E_LedTapTempo = 20,
      E_LedDevice = 21,
      E_LedDownArrow = 22,
      E_LedRightArrow = 23,
      E_LedOctaveDown = 24,
      E_LedPageLeft = 25,
      E_LedMix = 26,
      E_LedUndo = 27,
      E_LedLayout = 28,
      E_LedDuplicate = 29,
      E_LedNew = 30,
      E_LedQuantize = 31,
      E_LedAutomate = 32,
      E_LedSelect = 33,
      E_LedPageRight = 34,
      E_LedMetronome = 35,
      E_LedUser = 36,
      E_LedOctaveUp = 37,
      E_LedT = 38,
      E_TouchSurfacE_Led = 39,
      E_LedUpArrow = 40,
      E_LedRecord = 41,
      E_LedB = 42,
      E_LedMaster = 43,
      E_LedClip = 44,
      E_LedDoubleLoop = 45
   };
   Q_ENUM_NS(Id)
} // namespace Led

namespace Button3d
{
   Q_NAMESPACE
   enum Id
   {
      E_BtnSil = 0
   };
   Q_ENUM_NS(Id)
} // namespace Button3d

namespace Button
{
   Q_NAMESPACE
   enum Id
   {
      E_BtnLayout = 0,
      E_BtnSel = 1,
      E_BtnSession = 2,
      E_BtnQuantize = 3,
      E_BtnNote = 4,
      E_BtnMute = 5,
      E_BtnUser = 6,
      E_BtnDoubleLoop = 7,
      E_BtnStopClip = 8,
      E_BtnLeftArrow = 9,
      E_BtnMetronome = 10,
      E_BtnSelect = 11,
      E_BtnMix = 12,
      E_BtnB = 13,
      E_BtnDuplicate = 14,
      E_BtnPageRight = 15,
      E_BtnT = 16,
      E_BtnPlay = 17,
      E_BtnAccent = 18,
      E_BtnShift = 19,
      E_BtnSetup = 20,
      E_BtnUpArrow = 21,
      E_BtnFixedLength = 22,
      E_BtnSolo = 23,
      E_BtnTapTempo = 24,
      E_BtnDevice = 25,
      E_BtnScale = 26,
      E_BtnAddDevice = 27,
      E_BtnAutomate = 28,
      E_BtnOctaveDown = 29,
      E_BtnUndo = 30,
      E_BtnConvert = 31,
      E_BtnAddTrack = 32,
      E_BtnOctaveUp = 33,
      E_BtnRightArrow = 34,
      E_BtnDelete = 35,
      E_BtnMaster = 36,
      E_BtnNew = 37,
      E_BtnPageLeft = 38,
      E_BtnRecord = 39,
      E_BtnRepeat = 40,
      E_BtnDownArrow = 41,
      E_BtnClip = 42,
      E_BtnBrowse = 43
   };
   Q_ENUM_NS(Id)
} // namespace Button

namespace Encoder
{
   Q_NAMESPACE
   enum Id
   {
      E_EncoderMetronome = 0,
      E_EncoderMain = 1,
      E_EncoderTempo = 2,
      E_Encoder = 3
   };
   Q_ENUM_NS(Id)
} // namespace Encoder

namespace TouchSurface
{
   Q_NAMESPACE
   enum Id
   {
      E_TouchStrip = 0
   };
   Q_ENUM_NS(Id)
} // namespace TouchSurface

class Push2Device : public QObject,
                    public fp::Button::CallbackIf,
                    public fp::Button3d::CallbackIf,
                    public fp::Encoder::CallbackIf,
                    public fp::TouchSurface::CallbackIf,
                    public fp::Led::ILedScene
{
   Q_OBJECT
public:
   explicit Push2Device(push2device::Push2Device& rPush2Device,
                        QObject* parent = Q_NULLPTR);

   void registerCB(typename fp::Button::CallbackIf& cb, const fp::Widget& w);
   void registerCB(typename fp::Button3d::CallbackIf& cb, const fp::Widget& w);
   void registerCB(typename fp::Encoder::CallbackIf& cb, const fp::Widget& w);
   void registerCB(typename fp::TouchSurface::CallbackIf& cb, const fp::Widget& w);
   void unRegisterCB(typename fp::Button::CallbackIf& cb, const fp::Widget& w);
   void unRegisterCB(typename fp::Button3d::CallbackIf& cb, const fp::Widget& w);
   void unRegisterCB(typename fp::Encoder::CallbackIf& cb, const fp::Widget& w);
   void unRegisterCB(typename fp::TouchSurface::CallbackIf& cb, const fp::Widget& w);
   void reserveLed(fp::Led::ILedScene& ledScene, const QColor& color, const fp::Widget& w);
   void reserveLedOfBtn(fp::Led::ILedScene& ledScene, const QColor& color, const fp::Widget& w);
   void releaseLed(fp::Led::ILedScene& ledScene, const fp::Widget& w);
   void setLed(fp::Led::ILedScene& ledScene, const QColor& color, const fp::Widget& w);
   void setLedOfBtn(fp::Led::ILedScene& ledScene, const QColor& color, const fp::Widget& w);

private:
   push2device::Push2Device& m_rPush2Device;
   void onPressStateChange(const fp::Button::PressState& pressState, const fp::Widget& w) final;
   void onPressStateChange(const fp::Button3d::StateData& data, const Widget& w) final;
   void onPositionEvents(const uint32_t& pressure, const Widget& w) final;
   void onIncrement(int32_t data, const fp::Widget& w) final;
   void onTouchStateChanged(fp::Encoder::TouchState touchState, const fp::Widget& w) final;
   void onTouchStateChanged(fp::TouchSurface::TouchState touchState, const Widget& w) final;
   void onPositionEvents(const fp::TouchSurface::PressData& data, const Widget& w) final;
   void onGotRevealed(const Widget& w) final {};
   void onGotHidden(const Widget& w) final {};

public slots:
   void onReserveLed(const QColor& color, Led::Id id, int x = 0, int y = 0);
   void onReserveLedOfBtn(const QColor& color, Button::Id id, int x = 0, int y = 0);
   void onSetLed(const QColor& color, Led::Id id, int x = 0, int y = 0);
   void onSetLedOfBtn(const QColor& color, Button::Id id, int x = 0, int y = 0);

public:
   Q_INVOKABLE ButtonPressState::Value getButtonState(Button::Id id, int x = 0, int y = 0) const;

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