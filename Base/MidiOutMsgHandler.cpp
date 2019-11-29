#include "MidiOutMsgHandler.h"

MidiOutMsgHandler::MidiOutMsgHandler(std::unique_ptr<midi::IMidiOutMedium>   pMedium,
                                     std::shared_ptr<MusicDeviceDescription> pDescr) noexcept:
    m_midiOut(std::move(pMedium)),
    m_pDescr(pDescr)
{
}

void MidiOutMsgHandler::sendSoundParameter(uint32_t voiceId, uint32_t parameterId, float value) noexcept
{
    if(!m_pDescr->soundSection) return;
    const auto& nrpn = m_pDescr->soundSection->parameters[parameterId].midi.nrpn;
    if(nrpn)
    {
        midi::Message<midi::NRPN> nrpnMsg(voiceId + 1);
        nrpnMsg.idMsb = nrpn->operator[](0);
        nrpnMsg.idLsb = nrpn->operator[](1);
        constexpr unsigned nrpnResBits = 7;
        constexpr unsigned nrpnResBitMask = (1 << nrpnResBits) - 1;
        constexpr auto range = (1 << (2 * nrpnResBits)) - 1;
        const unsigned v = value * range;
        nrpnMsg.valueMsb = (v >> nrpnResBits);
        nrpnMsg.valueLsb = (v & nrpnResBitMask);
        m_midiOut.send(nrpnMsg);
    }
    else
    {
        constexpr unsigned ccResBits = 7;
        constexpr unsigned ccResBitMask = (1 << ccResBits) - 1;
        auto& cc = m_pDescr->soundSection->parameters[parameterId].midi.cc;
        const auto range = (1 << (cc.size() * ccResBits)) - 1;
        unsigned v = value * range;
        for(unsigned i = 0; i < cc.size(); ++i)
        {
            const auto idx = cc.size() - 1 - i;
            midi::Message<midi::ControlChange> ccMsgToSend(voiceId + 1, cc[idx], v & ccResBitMask);
            v = (v >> ccResBits);

            if(2 == cc.size() && 0 == i)
            {
                if(m_lastSentCcMsb)
                {
                    if(*m_lastSentCcMsb == ccMsgToSend)
                    {
                        continue;
                    }
                }
                m_lastSentCcMsb = ccMsgToSend;
            }
            m_midiOut.controlParameter(ccMsgToSend.channel(),
                                       ccMsgToSend.controllerNumber(),
                                       ccMsgToSend.controllerValue());
        }
    }
}

void MidiOutMsgHandler::quarterNotePer24Tick() noexcept
{
    m_midiOut.send(midi::MidiMessage(midi::Message<midi::MsgType::Clock>()));
}

void MidiOutMsgHandler::startSequencer() noexcept
{
    m_midiOut.send(midi::MidiMessage(midi::Message<midi::MsgType::Start>()));
}

void MidiOutMsgHandler::stopSequencer() noexcept
{
    m_midiOut.send(midi::MidiMessage(midi::Message<midi::MsgType::Stop>()));
}

bool MidiOutMsgHandler::sendParameterDumpRequest() noexcept
{
    if(m_pDescr->soundSection && 
       m_pDescr->soundSection->parameter_dump_request)
    {
        if(m_pDescr->soundSection->parameter_dump_request->midi_sysex)
        {
            m_midiOut.sysEx(*m_pDescr->soundSection->parameter_dump_request->midi_sysex);
        }
        if(m_pDescr->soundSection->parameter_dump_request->midi_cc)
        {
            m_midiOut.controlParameter(1,
                                       m_pDescr->soundSection->parameter_dump_request->midi_cc->cc[0],
                                       m_pDescr->soundSection->parameter_dump_request->midi_cc->value);
        }
        return true;
    }
    return false;
}

void MidiOutMsgHandler::processMidiOutBuffers() noexcept
{
    m_midiOut.processBuffers();
}
