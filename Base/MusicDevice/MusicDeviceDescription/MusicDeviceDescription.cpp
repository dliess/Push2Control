#include "MusicDeviceDescription.h"

#include <fmt/format.h>

#include <cassert>
#include <exception>

#include "2DMatrixCheck.h"
#include "Overload.h"

template<typename T>
void checkSource(const std::vector<std::vector<T>>& matrix2D,
                 const ControllerDeviceWidgetDimension& widgetDim,
                 const std::string& eventName)
{
   const auto dimSource = util::dimOf(matrix2D);
   if (dimSource.first == 0 || dimSource.second == 0)
   {
      throw std::runtime_error(
         fmt::format("dimension is 0 in source of event '{}'", eventName));
   }
   if (dimSource.first > 1 || dimSource.second > 1)
   {
      if (dimSource.first != widgetDim.numRows)
      {
         throw std::runtime_error(fmt::format(
            "row dimension mismatch between widget and source of event '{}'", eventName));
      }
      if (dimSource.second != widgetDim.numColumns)
      {
         throw std::runtime_error(fmt::format(
            "column dimension mismatch between widget and source of event '{}'", eventName));
      }
      if (!util::isConsistent(matrix2D))
      {
         throw std::runtime_error(fmt::format(
            "source of event '{}' is an inconsistent matrix", eventName));
      }
   }
}

void MusicDeviceDescription::checkValidity() const
{
   if (controllerSection)
   {
      for (const auto& widget : controllerSection->widgets)
      {
         for (const auto& event : widget.events)
         {
            mpark::visit(
               util::overload{
                  [&widget](const ControllerDeviceEventPressRelease& evt) {
                     checkSource(evt.pressSource, widget.dimension, evt.name);
                     checkSource(evt.releaseSource, widget.dimension, evt.name);
                  },
                  [&widget](const ControllerDeviceEventContinousValue& evt) {
                     checkSource(evt.source, widget.dimension, evt.name);
                  },
                  [&widget](const ControllerDeviceEventIncremental& evt) {
                     checkSource(evt.source, widget.dimension, evt.name);
                  }},
               event);
         }
      }
   }
}
