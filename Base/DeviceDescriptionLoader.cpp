#include "DeviceDescriptionLoader.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <loguru.hpp>

#include "MusicDeviceDescription.h"
#include "MusicDeviceId.h"

DeviceDescriptionLoader::DeviceDescriptionLoader()
{
   const std::string mapFileName("MidiConfigs/usbMidiName2device.json");
   const std::string customMapFileName(
      "MidiConfigs/custom_usbMidiName2device.json");
   std::ifstream mapFile(mapFileName);
   std::ifstream customMapFile(customMapFileName);
   if (mapFile.fail())
   {
      throw std::runtime_error(
         fmt::format("could not find file '{}'", mapFileName));
   }
   if (customMapFile.fail())
   {
      LOG_F(INFO, "could not find custom file '{}'", customMapFileName);
   }
   try
   {
      mapFile >> m_jUsbMidiName2deviceMap;
      customMapFile >> m_jUsbMidiHubName2deviceMap;
   }
   catch (...)
   {
      LOG_F(ERROR, "ERROR at parsing '{}' or '{}'", mapFileName,
            customMapFileName);
   }
}

std::optional<std::string> DeviceDescriptionLoader::getConfigPath(
   const MusicDeviceId& musicDeviceId) const noexcept
{
   const auto hubIter = m_jUsbMidiHubName2deviceMap.find(musicDeviceId.toStr());
   if (m_jUsbMidiHubName2deviceMap.end() != hubIter)
   {
      return hubIter->get<std::string>();
   }
   const auto iter = m_jUsbMidiName2deviceMap.find(musicDeviceId.deviceName);
   if (m_jUsbMidiName2deviceMap.end() != iter)
   {
      return iter->get<std::string>();
   }
   return std::nullopt;
}

MusicDeviceDescription DeviceDescriptionLoader::load(
   const MusicDeviceId& musicDeviceId) const
{
   const auto configPath = getConfigPath(musicDeviceId);
   if (!configPath)
   {
      throw std::runtime_error(fmt::format(
         "Device {} could not be found in any map", musicDeviceId.toStr()));
   }

   const std::string devFilePath =
      fmt::format("MidiConfigs/Devices/{}/Config.json", *configPath);
   std::ifstream devFile(devFilePath);
   if (devFile.fail())
   {
      throw std::runtime_error(
         fmt::format("Config file {} not found", devFilePath));
   }

   nlohmann::json jDev;
   try
   {
      devFile >> jDev;
      jDev["configPath"] = *configPath; // TODO: find a nicer solution
   }
   catch (std::exception& e)
   {
      throw std::runtime_error(fmt::format("Parsing file {} failed, reason: {}",
                                           devFilePath, e.what()));
   }
   return jDev.get<MusicDeviceDescription>();
}