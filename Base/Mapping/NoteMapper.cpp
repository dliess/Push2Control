#include "NoteMapper.h"
#include "loguru.hpp"
#include <Overload.h>

NoteMapper::NoteMapper(MusicDeviceHolder::MusicDevices& rControllerDevices,
                       Instruments&                     rInstruments) noexcept :
    m_rControllerDevices(rControllerDevices),
    m_rInstruments(rInstruments)
{
    for(unsigned i = 0; i < m_rControllerDevices.size(); ++i)
    {
        auto &device = m_rControllerDevices.atIdx(i).second;
        device->registerForNoteChange([this](int presetId, bool on, int note, float velocity){
            onNoteChange(presetId, on, note, velocity);
        });
        device->registerForPitchBendChange([this](int presetId, int note, float pitchBend){
            onPitchBendChange(presetId, note, pitchBend);
        });
    }
}

NoteMapper::Settings NoteMapper::getSettings() const noexcept
{
    Settings settings;
    for(auto& [src, dst] : m_mapping)
    {
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

        settings.emplace_back(NoteMapperDescription{src, destSettings});
    }
    return settings;
}

void NoteMapper::setSettings(const Settings& settings) noexcept
{
    for(const auto& noteMapperDescr : settings)
    {
        try{
            auto dest = mpark::visit(util::overload{
                [this](const KitSoundIdSettings& rKitSndIdSettings) -> MappingDestination{
                    MappingDestination mappingDestination;
                    auto kitIndex = findIndexOfElementByName(m_rInstruments.kitInstruments, rKitSndIdSettings.kitInstrumentId);
                    if(!kitIndex)
                    {
                        LOG_F(WARNING, "could not find '{}' in KitInstruments", rKitSndIdSettings.kitInstrumentId);
                        throw std::exception();
                    }
                    mappingDestination.emplace<KitSoundId>(KitSoundId{*kitIndex, rKitSndIdSettings.soundIndex});
                    return mappingDestination;
                },
                [this](const MelodicInstrumentIdSettings& rMelodicInstrumentIdSettings) -> MappingDestination{
                    MappingDestination mappingDestination;
                    auto index = findIndexOfElementByName(m_rInstruments.melodicInstruments, rMelodicInstrumentIdSettings.melodicInstrumentId);
                    if(!index)
                    {
                        LOG_F(WARNING, "could not find '{}' in melodicInstruments", rMelodicInstrumentIdSettings.melodicInstrumentId);
                        throw std::exception();
                    }
                    mappingDestination.emplace<MelodicInstrumentId>(MelodicInstrumentId{*index});
                    return mappingDestination;
                }
            }, noteMapperDescr.destination);

            m_mapping[noteMapperDescr.source] = dest;
        }catch(...) { continue; }
    }
}

void NoteMapper::onNoteChange(int presetId, bool on, int note, float velocity) noexcept
{

}

void NoteMapper::onPitchBendChange(int presetId, int note, float pitchBend) noexcept
{

}
