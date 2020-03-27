#ifndef CONTROLLER_DEVICE_EVENTS_H
#define CONTROLLER_DEVICE_EVENTS_H

#include <optional>

namespace base::ctrldev
{

struct WidgetId
{
    int id;
    int x;
    int y;
};

struct EncoderEvent
{
    WidgetId id;
    int      increment;
};

struct ButtonEvent
{
    WidgetId id;
    bool     on;
    std::optional<float> velocity;
    std::optional<float> xPos;
    std::optional<float> yPos;
};

struct ButtonEvent
{
    WidgetId id;
    bool     on;
    std::optional<float> velocity;
    std::optional<float> xPos;
    std::optional<float> yPos;
};

struct ButtonEvent
{
    WidgetId id;
    bool     on;
    std::optional<float> velocity;
    std::optional<float> xPos;
    std::optional<float> yPos;
};

struct ButtonAftertouch
{
    WidgetId id;
    float    value;
};

struct SurfaceDrag1D
{
    WidgetId id;
    float    pos;
};

struct SurfaceDrag2D
{
    WidgetId id;
    float    xPos;
    float    yPos;
};

} // namespace base

#endif