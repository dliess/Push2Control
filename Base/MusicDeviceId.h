#ifndef MUSIC_DEVICE_ID_H
#define MUSIC_DEVICE_ID_H

#include <string>
#include <Meta.h>

struct MusicDeviceId
{
    std::string deviceName;
    std::string portName;
    inline static const std::string ANY_PORT = "";
    MusicDeviceId() = default;
    MusicDeviceId(std::string deviceName, std::string portName) noexcept:
        deviceName(std::move(deviceName)), portName(std::move(portName)) 
    {}
    MusicDeviceId(std::string _deviceName) noexcept
    {
        auto n = _deviceName.find("@");
        if(n == std::string::npos)
        {
            deviceName = _deviceName;
            return;
        }
        deviceName = _deviceName.substr(0, n);
        portName = _deviceName.substr(n+1);
    }
    bool operator==(const MusicDeviceId& rhs) const noexcept
    {
        return (deviceName == rhs.deviceName) && 
               (portName.empty() || rhs.portName.empty() || portName == rhs.portName); 
    }
    std::string toStr() const noexcept { return deviceName + "@" + portName; }
};

namespace meta
{

template <>
inline auto registerMembers<MusicDeviceId>()
{
   return members(
      member("deviceName", &MusicDeviceId::deviceName),
      member("portName",   &MusicDeviceId::portName)
   );
}


}

namespace std
{
  template <>
  struct hash<MusicDeviceId>
  {
    size_t operator()(const MusicDeviceId& rMusicDevice) const noexcept
    {
        hash<string> hasher;
        return hasher(rMusicDevice.deviceName) ^ hasher(rMusicDevice.portName);
    }
  };
} // namespace std

#endif