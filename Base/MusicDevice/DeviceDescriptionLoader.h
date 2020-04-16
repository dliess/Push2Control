#ifndef DEVICE_DESCRIPTION_LOADER_H
#define DEVICE_DESCRIPTION_LOADER_H

#include <string>

#include "json.hpp"

class MusicDeviceDescription;
struct MusicDeviceId;

class DeviceDescriptionLoader
{
public:
   DeviceDescriptionLoader(const std::string& configDir);
   MusicDeviceDescription load(const MusicDeviceId& musicDeviceId) const;

private:
   std::string m_configDir;
   using json = nlohmann::json;
   json m_jUsbMidiName2deviceMap;
   json m_jUsbMidiHubName2deviceMap;

   std::optional<std::string> getConfigPath(
      const MusicDeviceId& musicDeviceId) const noexcept;
};

#endif