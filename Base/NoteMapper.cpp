#include "NoteMapper.h"
#include <Overload.h>

NoteMapper::NoteMapper(MusicDeviceHolder::MusicDevices& rControllerDevices,
                       Instruments&                     rInstruments) noexcept :
    m_rControllerDevices(rControllerDevices),
    m_rInstruments(rInstruments)
{
}

NoteMapper::Settings NoteMapper::getSettings() const noexcept
{
    Settings settings;
    for(auto& [src, dst] : m_mapping)
    {
        const MusicDeviceId cntrlDeviceId = m_rControllerDevices.at(src.deviceIndex).first;
        ControllerDeviceIdSettings srcSettings{cntrlDeviceId, src.presetIndex};

        auto destSettings = mpark::visit(util::overload{
            [this](const KitSoundId& rKitSndId) -> decltype(NoteMapperDescription::destination) {
                decltype(NoteMapperDescription::destination) destination;
                destination.emplace<KitSoundIdSettings>(KitSoundIdSettings{
                    m_rInstruments.kitInstruments[rKitSndId.kitIndex].name(),
                    rKitSndId.soundIndex
                });
                return destination;
            },
            [this](const MelodicInstrumentId& rMelodicInstrumentId) -> decltype(NoteMapperDescription::destination){
                decltype(NoteMapperDescription::destination) destination;
                destination.emplace<MelodicInstrumentIdSettings>(MelodicInstrumentIdSettings{
                    m_rInstruments.melodicInstruments[rMelodicInstrumentId.index].name()
                });
                return destination;
            }
        }, dst);

        settings.emplace_back(NoteMapperDescription{srcSettings, destSettings});
    }
    return settings;
}

void NoteMapper::setSettings(const Settings& settings) noexcept
{

}
