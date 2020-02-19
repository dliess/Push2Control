#ifndef MUSIC_DEVICE_FACTORY_H
#define MUSIC_DEVICE_FACTORY_H

class MusicDeviceHolder;

namespace base
{

class Instruments;

class MusicDeviceFactory
{
public:
    MusicDeviceFactory(MusicDeviceHolder& rMusicDeviceHolder,
                       Instruments&       rInstruments) noexcept;
private:
    MusicDeviceHolder& m_rMusicDeviceHolder;
    Instruments&       m_rInstruments;
};

} // namespace base

#endif