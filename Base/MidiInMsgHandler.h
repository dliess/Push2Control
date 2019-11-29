#ifndef MIDI_IN_MSG_HANDLER_H
#define MIDI_IN_MSG_HANDLER_H

#include "MusicDeviceDescription.h"
#include "Midi1Input.h"
#include <cstdint>
#include <array>
#include <vector>
#include <optional>
#include <memory>

namespace midi { class Router; }
namespace midi { class Dumper; }

class MidiInMsgHandler
{
public:
    MidiInMsgHandler(std::unique_ptr<midi::IMidiInMedium>    pMedium,
                     std::shared_ptr<MusicDeviceDescription> pDescr);

    using Cb = std::function<void(uint32_t voiceId, uint32_t parameterId, float value)>;
    void registerForSoundParameter(Cb cb) noexcept;
    void registerForControlParameter(Cb cb) noexcept;

    friend midi::Router;
    friend midi::Dumper;
private:
    midi::Midi1Input m_midiIn;
    std::shared_ptr<MusicDeviceDescription> m_pDescr;

    struct CC2ParamIdMappingEntry
    {
        static constexpr int NONE = -1;
        MusicDeviceDescription::Type deviceType{MusicDeviceDescription::Type::Unknown};
        int      parameterId{NONE};
        int      otherCCIdx{NONE};
    };
    std::array<CC2ParamIdMappingEntry,128>            m_cc2IdCache;
    std::optional<midi::Message<midi::ControlChange>> m_lastReceivedCcMsb;
    void recvParameter(midi::Message<midi::ControlChange> msg) noexcept;
    void recvParameter(const midi::Message<midi::ControlChange>& ccMsgMsb,
                       const midi::Message<midi::ControlChange>& ccMsgLsb) noexcept;
    bool isCcMsb(midi::Message<midi::ControlChange> ccMsg) const noexcept;

    std::vector<Cb> m_soundParameterCbs;
    std::vector<Cb> m_controlParameterCbs;
};

#endif