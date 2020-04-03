#ifndef CONTROLLER_DEVICE_EVENTS_H
#define CONTROLLER_DEVICE_EVENTS_H

#include <mpark/variant.hpp>

namespace base::ctrldev
{

constexpr int INVALID_IDX = -1;
struct WidgetCoord
{
   int row{INVALID_IDX};
   int col{INVALID_IDX};
};

struct WidgetId
{
   int id{INVALID_IDX};
   WidgetCoord coord;
};

using PressReleaseType   = float;   // -1.0 to 1.0
using ContinousValueType = float;   // 0.0 to 1.0
using IncrementType      = int;

struct PressReleaseEvent
{
   WidgetId wId;
   PressReleaseType value;
};

struct ContinousValueEvent
{
   WidgetId wId;
   IncrementType value;
};

struct IncrementEvent
{
   WidgetId wId;
   ContinousValueType value;
};

using EventType =
    mpark::variant<PressReleaseEvent, ContinousValueEvent, IncrementEvent>;

}   // namespace base::ctrldev

#endif