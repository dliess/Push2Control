#include "MusicDeviceFactory.h"
#include "MusicDeviceHolder.h"
#include "Base.h" // Instruments

base::MusicDeviceFactory::MusicDeviceFactory(MusicDeviceHolder& rMusicDeviceHolder, 
                                             Instruments&       rInstruments) noexcept :
    m_rMusicDeviceHolder(rMusicDeviceHolder),
    m_rInstruments(rInstruments)
{
}
