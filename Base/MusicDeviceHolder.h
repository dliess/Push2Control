#ifndef MUSIC_DEVICE_HOLDER_H
#define MUSIC_DEVICE_HOLDER_H

#include "Midi1Input.h"
#include "Midi1Output.h"
#include "MidiMessage.h"
#include "IMidiInMedium.h"
#include "IMidiOutMedium.h"
#include "MusicDevice.h"
#include "MusicDeviceId.h"
#include "DeviceDescriptionLoader.h"
#include "KeyValueVector.h"

#include <cstdint>
#include <vector>
#include <limits>
#include <optional>
#include <memory>
#include <string>

class MusicDeviceHolder
{
public:
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

    void addMidiInputMedium(std::unique_ptr<midi::IMidiInMedium> pMedium);
    void addMidiOutputMedium(std::unique_ptr<midi::IMidiOutMedium> pMedium);

private:
    DeviceDescriptionLoader  deviceLoader;
    std::shared_ptr<MusicDevice> findMusicDevice(const MusicDeviceId& deviceId) const noexcept;
    std::shared_ptr<MusicDevice> createAndInsertMusicDevice(const MusicDeviceId&  deviceId,
                                                            const std::string&    deviceName);
};

#endif