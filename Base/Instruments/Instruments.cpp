#include "Instruments.h"

#include "MusicDeviceHolder.h"
#include <loguru.hpp>

Instruments::Settings Instruments::getSettings() const noexcept
{
   return *this;
}

void Instruments::setSettings(const Settings& settings) noexcept
{
   *this = settings;
}