#ifndef MUSIC_DEVICE_HOLDER_H
#define MUSIC_DEVICE_HOLDER_H

#include "MusicDevice.h"
#include "KeyValueVector.h"
#include <memory>

struct MusicDeviceHolder
{
    MusicDeviceHolder();
    std::shared_ptr<MusicDevice>& controllerDevice(int index) {return controllerDevices.atIdx(index).second;};
    std::shared_ptr<MusicDevice>& soundDevice(int index) {return soundDevices.atIdx(index).second;};
    const std::shared_ptr<MusicDevice>& controllerDevice(int index) const {return controllerDevices.atIdx(index).second;};
    const std::shared_ptr<MusicDevice>& soundDevice(int index) const {return soundDevices.atIdx(index).second;};

    using MusicDevices = util::KeyValueVector<MusicDeviceId, std::shared_ptr<MusicDevice>>;

    MusicDevices musicDevices;
    MusicDevices controllerDevices;
    MusicDevices soundDevices;
    MusicDevices transportCmdDrains;
};

#endif