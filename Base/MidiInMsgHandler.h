#ifndef MIDI_IN_MSG_HANDLER_H
#define MIDI_IN_MSG_HANDLER_H

#include "MusicDeviceDescription.h"
#include "ControllerDeviceEvents.h"
#include "Midi1Input.h"
#include <mpark/variant.hpp>
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
    void initMappingCaches() noexcept;
    void handleIncomingCcMsg(const midi::Message<midi::ControlChange>& ccMsg) noexcept;
    void handleIncomingNRPNMsg(const midi::Message<midi::NRPN>& nrpnMsg) noexcept;
    void handleIncomingNoteOnMsg(const midi::Message<midi::NoteOn>& noteOnMsg) noexcept;
    void handleIncomingNoteOffMsg(const midi::Message<midi::NoteOff>& noteOffMsg) noexcept;

    midi::Midi1Input m_midiIn;
    std::shared_ptr<MusicDeviceDescription> m_pDescr;

    static constexpr int INVALID_IDX = -1;
    struct SoundDevParameterId
    {
        int parameterId{INVALID_IDX};
    };
    struct ControllerEventId
    {
        base::ctrldev::WidgetId widgetId;
        int                     eventId{INVALID_IDX};
    };
    using CcMapDest = mpark::variant<SoundDevParameterId, ControllerEventId>;
    std::array<std::optional<CcMapDest>, 128>         m_cc2IdCache;
    std::array<std::optional<ControllerEventId>, 128> m_note2IdCache;
    std::optional<midi::Message<midi::ControlChange>> m_lastReceivedCcMsb;

    void recvParameter(midi::Message<midi::ControlChange> msg) noexcept;
    void recvParameter(const midi::Message<midi::ControlChange>& ccMsgMsb,
                       const midi::Message<midi::ControlChange>& ccMsgLsb) noexcept;
    bool isCcMsb(midi::Message<midi::ControlChange> ccMsg) const noexcept;

    std::vector<Cb> m_soundParameterCbs;
    std::vector<Cb> m_controlParameterCbs;
};

#endif