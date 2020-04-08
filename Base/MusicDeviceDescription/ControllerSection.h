#ifndef CONTROLLER_SECTION_H
#define CONTROLLER_SECTION_H

#include <string>
#include <mpark/variant.hpp>

struct ControllerDeviceWidgetDimension
{
   int numRows;
   int numColumns;
};

struct ControllerDeviceEventIncremental
{
   std::string name;
};

struct ControllerDeviceEventPressRelease
{
   std::string name;
};

struct ControllerDeviceEventContinousValue
{
   std::string name;
};

using ControllerDeviceEvent = mpark::variant<
   ControllerDeviceEventIncremental,
   ControllerDeviceEventPressRelease,
   ControllerDeviceEventContinousValue
>;

struct ControllerDeviceWidget
{
   std::string                        name;
   ControllerDeviceWidgetDimension    dimension;
   std::vector<ControllerDeviceEvent> events;
};

struct ControllerSection
{
   int                                 numPresets;
   std::vector<ControllerDeviceWidget> widgets;
};

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
auto getClassNameOrIndex<ControllerDeviceEventIncremental>(int i) noexcept
{
   return "Incremental";
}

template <>
inline auto registerMembers<ControllerDeviceEventIncremental>()
{
   return members(
      member("name", &ControllerDeviceEventIncremental::name)
   );
}

template <>
auto getClassNameOrIndex<ControllerDeviceEventPressRelease>(int i) noexcept
{
   return "PressRelease";
}

template <>
inline auto registerMembers<ControllerDeviceEventPressRelease>()
{
   return members(
      member("name", &ControllerDeviceEventPressRelease::name)
   );
}

template <>
auto getClassNameOrIndex<ControllerDeviceEventContinousValue>(int i) noexcept
{
   return "ContinousValue";
}

template <>
inline auto registerMembers<ControllerDeviceEventContinousValue>()
{
   return members(
      member("name", &ControllerDeviceEventContinousValue::name)
   );
}

template <>
inline auto registerMembers<ControllerDeviceWidget>()
{
   return members(
      member("name", &ControllerDeviceWidget::name),
      member("dimension", &ControllerDeviceWidget::dimension),
      member("events", &ControllerDeviceWidget::events)
   );
}

template <>
inline auto registerMembers<ControllerSection>()
{
   return members(
      member("numPresets", &ControllerSection::numPresets),
      member("widgets", &ControllerSection::widgets)
   );
}

} // namespace meta


#endif