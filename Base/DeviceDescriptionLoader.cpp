#include "DeviceDescriptionLoader.h"
#include "MusicDeviceDescription.h"
#include <iostream>
#include <fstream>
#include <loguru.hpp>

DeviceDescriptionLoader::DeviceDescriptionLoader()
{
    const std::string mapFileName("MidiConfigs/usbMidiName2device.json");
    const std::string customMapFileName("MidiConfigs/custom_usbMidiName2device.json");
    std::ifstream mapFile(mapFileName);
    std::ifstream customMapFile(customMapFileName);
    if(mapFile.fail())
    {
        LOG_F(ERROR, "could not find file '{}'", mapFileName);
        return;
    }
    if(customMapFile.fail())
    {
        LOG_F(ERROR, "could not find custom file '{}'", customMapFileName);
        return;
    }
    try{
        mapFile >> m_jUsbMidiName2deviceMap;
        customMapFile >> m_jCustomUsbMidiName2deviceMap;
    }catch(...)
    {
        LOG_F(ERROR, "ERROR at parsing '{}' or '{}'", mapFileName, customMapFileName);
    }
}

void DeviceDescriptionLoader::load(const std::string& usbMidiDeviceName, MusicDeviceDescription& rDescr) const
{
    nlohmann::json jDev;
    if(!loadByMap(m_jCustomUsbMidiName2deviceMap, usbMidiDeviceName, jDev))
    {
        loadByMap(m_jUsbMidiName2deviceMap, usbMidiDeviceName, jDev);
    }
    try{
        rDescr = jDev.get<MusicDeviceDescription>();
    }
    catch(std::runtime_error& e)
    {
        LOG_F(ERROR, "{}", e.what());
        rDescr.productName = usbMidiDeviceName;
    }
    catch(...)
    {
        LOG_F(ERROR, "Unknown exception caught");
        rDescr.productName = usbMidiDeviceName;
    }
}

bool DeviceDescriptionLoader::loadByMap(const nlohmann::json& rMap,
                                        const std::string&    usbMidiDeviceName,
                                        nlohmann::json&       jDevDescr)
{
    if(rMap.find(usbMidiDeviceName) != rMap.end())
    {
        //TODO: exception handling
        try{
            const std::string configPath = rMap[usbMidiDeviceName].get<std::string>();
            const std::string devFilePath = "MidiConfigs/Devices/" + configPath + "/Config.json";
            std::ifstream devFile(devFilePath);
            if(!devFile.fail())
            {
                devFile >> jDevDescr;
                jDevDescr["configPath"] = configPath;
                return true;
            }
        }catch(...)
        {
            return false;
        }
    }
    return false;
}