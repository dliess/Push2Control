#ifndef DEVICE_DESCRIPTION_LOADER_H
#define DEVICE_DESCRIPTION_LOADER_H

#include <string>

#include "json.hpp"

class MusicDeviceDescription;
struct MusicDeviceId;

class DeviceDescriptionLoader
{
public:
   DeviceDescriptionLoader();
   MusicDeviceDescription load(const MusicDeviceId& musicDeviceId) const;

private:
   using json = nlohmann::json;
   json m_jUsbMidiName2deviceMap;
   json m_jUsbMidiHubName2deviceMap;

   std::optional<std::string> getConfigPath(
      const MusicDeviceId& musicDeviceId) const noexcept;
};

#endif