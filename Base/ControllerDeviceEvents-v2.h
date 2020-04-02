#ifndef CONTROLLER_DEVICE_EVENTS_H
#define CONTROLLER_DEVICE_EVENTS_H

#include <mpark/variant.hpp>

namespace base::ctrldev
{

struct WidgetCoord
{
    int row;
    int col;
};

struct WidgetId
{
    int         id;
    WidgetCoord coord;
};

struct AbsoluteParameter : public WidgetId
{
    float value;
};

struct EncoderEvent : public WidgetId
{
    int increment;
};

struct ButtonPressEvent : public WidgetId
{
    bool on;
};

struct TouchedEvent : public WidgetId
{
    bool touched;
};

struct ButtonPressVelocityEvent : public AbsoluteParameter {};
struct ButtonReleaseVelocityEvent : public AbsoluteParameter {};
struct ButtonAftertouch : public AbsoluteParameter {};
struct PosXEvent : public AbsoluteParameter {};
struct PosYEvent : public AbsoluteParameter {};
struct PosZEvent : public AbsoluteParameter {};
struct RotAEvent : public AbsoluteParameter {};
struct RotBEvent : public AbsoluteParameter {};
struct RotCEvent : public AbsoluteParameter {};

using EventType = mpark::variant<
    EncoderEvent,
    ButtonPressEvent,
    TouchedEvent,
    ButtonPressVelocityEvent,
    ButtonReleaseVelocityEvent,
    ButtonAftertouch,
    PosXEvent,
    PosYEvent,
    PosZEvent,
    RotAEvent,
    RotBEvent,
    RotCEvent
>;

} // namespace base::ctrldev

#endif