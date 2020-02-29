#include "Instruments.h"

Instruments::Settings Instruments::getSettings() const noexcept
{
   return *this;
}

void Instruments::setSettings(const Settings& settings) noexcept
{
   *this = settings;
}
