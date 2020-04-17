#ifndef CONTROLLER_DEVICE_EVENTS_REFLECT_H
#define CONTROLLER_DEVICE_EVENTS_REFLECT_H

#include "JsonCast.h"
#include "Meta.h"

namespace base::ctrldev
{
#include "JsonCastNamespaceFix.h"
} // namespace base::ctrldev

namespace meta
{
template<>
inline auto registerMembers<base::ctrldev::WidgetCoord>()
{
   return members(member("row", &base::ctrldev::WidgetCoord::row),
                  member("col", &base::ctrldev::WidgetCoord::col));
}

template<>
inline auto registerMembers<base::ctrldev::EventId>()
{
   return members(member("widgetId", &base::ctrldev::EventId::widgetId),
                  member("widgetCoord", &base::ctrldev::EventId::widgetCoord),
                  member("eventId", &base::ctrldev::EventId::eventId));
}

template<>
inline auto getClassNameOrIndex<base::ctrldev::EventId>(int i) noexcept
{
   return "EventId";
}

template<>
inline auto registerMembers<base::ctrldev::PressReleaseType>()
{
   return members(member("value", &base::ctrldev::PressReleaseType::value));
}

template<>
inline auto getClassNameOrIndex<base::ctrldev::PressReleaseType>(int i) noexcept
{
   return "PressReleaseType";
}

template<>
inline auto registerMembers<base::ctrldev::ContinousValueType>()
{
   return members(member("value", &base::ctrldev::ContinousValueType::value));
}

template<>
inline auto getClassNameOrIndex<base::ctrldev::ContinousValueType>(int i) noexcept
{
   return "ContinousValueType";
}

template<>
inline auto registerMembers<base::ctrldev::IncrementType>()
{
   return members(member("value", &base::ctrldev::IncrementType::value));
}

template<>
inline auto getClassNameOrIndex<base::ctrldev::IncrementType>(int i) noexcept
{
   return "IncrementType";
}

template<>
inline auto registerMembers<base::ctrldev::Event>()
{
   return members(
      member("id", &base::ctrldev::Event::id),
      member("value", &base::ctrldev::Event::value)
   );
}

} // namespace meta

#endif