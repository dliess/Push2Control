#ifndef CONTROLLER_SECTION_META_H
#define CONTROLLER_SECTION_META_H

#include "Meta.h"
#include "JsonCast.h"

namespace meta
{

template <>
inline auto registerMembers<ControllerDeviceWidgetDimension>()
{
   return members(
      member("numRows", &ControllerDeviceWidgetDimension::numRows),
      member("numColumns", &ControllerDeviceWidgetDimension::numColumns)
   );
}

template <>
inline auto getClassNameOrIndex<ControllerDeviceEventIncremental>(int i) noexcept
{
   return "Incremental";
}

template <>
inline auto registerMembers<ControllerDeviceEventIncremental>()
{
   return members(
      member("name", &ControllerDeviceEventIncremental::name),
      member("source", &ControllerDeviceEventIncremental::source)
   );
}

template <>
inline auto getClassNameOrIndex<ControllerDeviceEventPressRelease>(int i) noexcept
{
   return "PressRelease";
}

template <>
inline auto registerMembers<ControllerDeviceEventPressRelease>()
{
   return members(
      member("name", &ControllerDeviceEventPressRelease::name),
      member("sourceHasInvertedLogic", &ControllerDeviceEventPressRelease::sourceHasInvertedLogic),
      member("pressSource", &ControllerDeviceEventPressRelease::pressSource),
      member("releaseSource", &ControllerDeviceEventPressRelease::releaseSource)
   );
}

template <>
inline auto getClassNameOrIndex<ControllerDeviceEventContinousValue>(int i) noexcept
{
   return "ContinousValue";
}

template <>
inline auto registerMembers<ControllerDeviceEventContinousValue>()
{
   return members(
      member("name", &ControllerDeviceEventContinousValue::name),
      member("source", &ControllerDeviceEventContinousValue::source)
   );
}

template <>
inline auto getClassNameOrIndex<ControllerDeviceEventRelativeValue>(int i) noexcept
{
   return "RelativeValue";
}

template <>
inline auto registerMembers<ControllerDeviceEventRelativeValue>()
{
   return members(
      member("name", &ControllerDeviceEventRelativeValue::name),
      member("source", &ControllerDeviceEventRelativeValue::source)
   );
}

template <>
inline auto registerMembers<ControllerDeviceWidget>()
{
   return members(
      member("name", &ControllerDeviceWidget::name),
      member("dimension", &ControllerDeviceWidget::dimension),
      member("mpe", &ControllerDeviceWidget::mpe),
      member("events", &ControllerDeviceWidget::events)
   );
}

template <>
inline auto registerMembers<ControllerSection>()
{
   return members(
      member("widgets", &ControllerSection::widgets)
   );
}

} // namespace meta


#endif