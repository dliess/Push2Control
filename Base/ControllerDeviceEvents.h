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
   int                        widgetId;
   std::optional<WidgetCoord> widgetCoord;
   int                        eventId;
};

struct PressReleaseType
{
   float value; // -1.0 to 1.0
};  
struct ContinousValueType
{
   float value;   // 0.0 to 1.0
};
struct IncrementType
{
   int value;
};
using EventValue = mpark::variant<mpark::monostate,
                                  PressReleaseType, 
                                  ContinousValueType,
                                  IncrementType>;

struct Event
{
   EventId    id;
   EventValue value;
};

} // namespace base::ctrldev

#endif