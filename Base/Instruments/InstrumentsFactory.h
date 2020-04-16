#ifndef INSTRUMENTS_FACTORY_H
#define INSTRUMENTS_FACTORY_H

#include <memory>

struct Instruments;
struct MusicDeviceHolder;
struct MusicDevice;

namespace base
{
class InstrumentsFactory
{
public:
   InstrumentsFactory(Instruments& rInstruments,
                      MusicDeviceHolder& rMusicDeviceHolder) noexcept;

private:
   Instruments& m_rInstruments;
private:
    void add(std::shared_ptr<MusicDevice> pMusicDevice);
    void remove(std::shared_ptr<MusicDevice> pMusicDevice);
};

} // namespace base
#endif