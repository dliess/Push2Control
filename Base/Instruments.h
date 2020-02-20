#ifndef INSTRUMENTS_H
#define INSTRUMENTS_H

#include "KitInstrument.h"
#include "MelodicInstrument.h"
#include "Settings.h"
#include "Meta.h"
#include <vector>


struct Instruments : public utils::Settings<Instruments>
{
    // ============== Settings ===============
    using Settings = Instruments;
    Settings getSettings() const noexcept;
    void setSettings(const Settings& settings) noexcept;
    // =======================================
    std::vector<KitInstrument>     kitInstruments;
    std::vector<int> melodicInstruments;
    //std::vector<MelodicInstrument> melodicInstruments;
};

namespace meta
{

template <>
inline auto registerMembers<Instruments>()
{
    return members(
        member("kitInstruments", &Instruments::kitInstruments),
        member("melodicInstruments", &Instruments::melodicInstruments)
    );
}

} // namespace meta

#endif