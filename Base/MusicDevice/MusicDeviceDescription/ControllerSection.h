#ifndef CONTROLLER_SECTION_H
#define CONTROLLER_SECTION_H

#include <string>
#include <mpark/variant.hpp>
#include "MidiMessageIds.h"

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
   std::optional<bool> sourceHasInvertedLogic;
   std::vector<std::vector<MidiMessageId>> pressSource;
   std::vector<std::vector<MidiMessageId>> releaseSource;
};

struct ControllerDeviceEventContinousValue
{
   std::string name;
   std::vector<std::vector<MidiMessageId>> source;
};

struct ControllerDeviceEventRelativeValue
{
   std::string name;
   std::vector<std::vector<MidiMessageId>> source;
};

using ControllerDeviceEvent = mpark::variant<
   ControllerDeviceEventIncremental,
   ControllerDeviceEventPressRelease,
   ControllerDeviceEventContinousValue,
   ControllerDeviceEventRelativeValue
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

#include "ControllerSectionMeta.h"

#endif