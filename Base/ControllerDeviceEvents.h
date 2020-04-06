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
struct EventId
{
   int         widgetId;
   WidgetCoord widgetCoord;
   int         eventId;
};

using PressReleaseType   = float;   // -1.0 to 1.0
using ContinousValueType = float;   // 0.0 to 1.0
using IncrementType      = int;
using EventValue = mpark::variant<PressReleaseType, 
                                  ContinousValueType,
                                  IncrementType>;

struct Event
{
   EventId    id;
   EventValue value;
};

} // namespace base::ctrldev

#endif