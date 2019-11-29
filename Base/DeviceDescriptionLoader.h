#ifndef DEVICE_DESCRIPTION_LOADER_H
#define DEVICE_DESCRIPTION_LOADER_H

#include "json.hpp"
#include <string>

class MusicDeviceDescription;

class DeviceDescriptionLoader
{
public:
    DeviceDescriptionLoader();
    void load(const std::string& usbMidiDeviceName, MusicDeviceDescription& rDescr) const;
private:
    using json = nlohmann::json;
    json m_jUsbMidiName2deviceMap;
    json m_jCustomUsbMidiName2deviceMap;

    static bool loadByMap(const nlohmann::json& rMap,
                          const std::string& usbMidiDeviceName,
                          nlohmann::json& jDevDescr);
};

#endif