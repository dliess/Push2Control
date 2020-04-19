#ifndef CONTROLLER_SECTION_H
#define CONTROLLER_SECTION_H

#include <string>
#include <mpark/variant.hpp>
#include "MidiMessageIds.h"
#include "Meta.h"
#include "JsonCast.h"

struct ControllerDeviceWidgetDimension
{
   int numRows;
   int numColumns;
};

struct ControllerDeviceEventIncremental
{
   std::string name;
   std::vector<std::vector<MidiMessageId>> source;
};

struct ControllerDeviceEventPressRelease
{
   std::string name;
   std::vector<std::vector<MidiMessageId>> pressSource;
   std::vector<std::vector<MidiMessageId>> releaseSource;
};

struct ControllerDeviceEventContinousValue
{
   std::string name;
   std::vector<std::vector<MidiMessageId>> source;
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
   std::optional<bool>                mpe;
   std::vector<ControllerDeviceEvent> events;
};

struct ControllerSection
{
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