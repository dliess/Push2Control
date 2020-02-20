#ifndef MUSIC_DEVICE_FACTORY_H
#define MUSIC_DEVICE_FACTORY_H

#include "DeviceDescriptionLoader.h"
#include <memory>

struct MusicDeviceHolder;
struct MusicDeviceId;
struct MusicDevice;
struct Instruments;

namespace midi
{
class IMidiInMedium;
class IMidiOutMedium; 
} // namespace midi

namespace base
{


class MusicDeviceFactory
{
public:
    MusicDeviceFactory(MusicDeviceHolder& rMusicDeviceHolder,
                       Instruments&       rInstruments) noexcept;

    void addMidiInputMedium(std::unique_ptr<midi::IMidiInMedium> pMedium);
    void addMidiOutputMedium(std::unique_ptr<midi::IMidiOutMedium> pMedium);
#ifdef __INSERT_DUMMY_MIDI_DEVICES__
    void insertMusicDeviceDummies();
#endif

private:
    MusicDeviceHolder& m_rMusicDeviceHolder;
    Instruments&       m_rInstruments;
    DeviceDescriptionLoader  deviceLoader;
    std::shared_ptr<MusicDevice> findMusicDevice(const MusicDeviceId& deviceId) const noexcept;
    std::shared_ptr<MusicDevice> createAndInsertMusicDevice(const MusicDeviceId&  deviceId,
                                                            const std::string&    deviceName);
};

} // namespace base

#endif