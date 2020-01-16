#ifndef DEVICE_DESCRIPTION_LOADER_H
#define DEVICE_DESCRIPTION_LOADER_H

#include "json.hpp"
#include <string>

class MusicDeviceDescription;
struct MusicDeviceId;

class DeviceDescriptionLoader
{
public:
    DeviceDescriptionLoader();
    void load(const MusicDeviceId& musicDeviceId, MusicDeviceDescription& rDescr) const;
private:
    using json = nlohmann::json;
    json m_jUsbMidiName2deviceMap;
    json m_jUsbMidiHubName2deviceMap;

    static bool loadByMap(const nlohmann::json& rMap,
                         const std::string&    key,
                         nlohmann::json&       jDevDescr) noexcept;
};

#endif