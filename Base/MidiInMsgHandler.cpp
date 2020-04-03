#include "MidiInMsgHandler.h"

MidiInMsgHandler::MidiInMsgHandler(std::unique_ptr<midi::IMidiInMedium>    pMedium,
                                   std::shared_ptr<MusicDeviceDescription> pDescr):
    m_midiIn(std::move(pMedium)),
    m_pDescr(pDescr)
{
    initMappingCaches();

    m_midiIn.registerMidiInCb([this](const midi::MidiMessage& midiMsg){
        mpark::visit(midi::overload{
            [this](const midi::Message<midi::ControlChange>& ccMsg) { handleIncomingCcMsg(ccMsg); },
            [this](const midi::Message<midi::NRPN>& nrpnMsg) { handleIncomingNRPNMsg(nrpnMsg); },
            [this](const midi::Message<midi::NoteOn>& noteOnMsg) { handleIncomingNoteOnMsg(noteOnMsg); },
            [this](const midi::Message<midi::NoteOff>& noteOffMsg) { handleIncomingNoteOffMsg(noteOffMsg); },
            [](auto&& other) { /* Ignore them */ }
        }, midiMsg);
    });
}

void MidiInMsgHandler::initMappingCaches() noexcept
{
    for(unsigned ccIdx = 0; ccIdx < m_cc2IdCache.size(); ++ccIdx)
    {
        const unsigned numSoundDevParams = m_pDescr->soundSection ? m_pDescr->soundSection->parameters.size() : 0;
        for(unsigned paramIdx = 0; paramIdx < numSoundDevParams; ++paramIdx)
        {
            const auto& ccArr = m_pDescr->soundSection->parameters[paramIdx].midi.cc;
            for(unsigned ccSigIdx = 0; ccSigIdx < ccArr.size(); ++ccSigIdx)
            {
                if(ccArr[ccSigIdx] == ccIdx)
                {
                    m_cc2IdCache[ccIdx].deviceType = MusicDeviceDescription::Type::SoundDevice;
                    m_cc2IdCache[ccIdx].parameterId = paramIdx;
                    if(ccArr.size() == 2)
                    {
                        m_cc2IdCache[ccIdx].otherCCIdx = ccArr[(ccSigIdx + 1) % 2];
                    }
                }
            }
        }
        const unsigned numControllerDevParams = m_pDescr->controllerSection ? m_pDescr->controllerSection->parameters.size() : 0;
        for(unsigned paramIdx = 0; paramIdx < numControllerDevParams; ++paramIdx)
        {
            const auto& ccArr = m_pDescr->controllerSection->parameters[paramIdx].midi.cc;
            for(unsigned ccSigIdx = 0; ccSigIdx < ccArr.size(); ++ccSigIdx)
            {
                if(ccArr[ccSigIdx] == ccIdx)
                {
                    m_cc2IdCache[ccIdx].deviceType = MusicDeviceDescription::Type::ControllerDevice;
                    m_cc2IdCache[ccIdx].parameterId = paramIdx;
                    if(ccArr.size() == 2)
                    {
                        m_cc2IdCache[ccIdx].otherCCIdx = ccArr[(ccSigIdx + 1) % 2];
                    }
                }
            }
        }
    }
}

void MidiInMsgHandler::handleIncomingCcMsg(const midi::Message<midi::ControlChange>& ccMsg) noexcept
{
    const auto& entry = m_cc2IdCache[ccMsg.controllerNumber()];
    if(!entry) return;

    mpark::visit(midi::overload{
        [this, &ccMsg](const SoundDevParameterId& id){
            for(auto& cb : m_soundParameterCbs)
                cb(ccMsg.channel() - 1, id.parameterId, ccMsg.getRelativeValue());
        },
        [this](const ControllerEventId& id){
            
        }
    }, *entry);
}

void MidiInMsgHandler::handleIncomingNRPNMsg(const midi::Message<midi::NRPN>& nrpnMsg) noexcept
{
    const float value = ((nrpnMsg.valueMsb << 7) + nrpnMsg.valueLsb)  / float(128 * 128);
    if(m_pDescr->soundSection)
    {
        for(int paramIdx = 0; paramIdx < m_pDescr->soundSection->parameters.size(); ++paramIdx){
            const auto& nrpn = m_pDescr->soundSection->parameters[paramIdx].midi.nrpn;
            if(nrpn &&
            nrpn->operator[](0) == nrpnMsg.idMsb &&
            nrpn->operator[](1) == nrpnMsg.idLsb)
            {
                for(auto& cb : m_soundParameterCbs) cb(nrpnMsg.channelNr - 1, paramIdx, value);
                return;
            }
        }
    }
    if(m_pDescr->controllerSection)
    {
        for(int paramIdx = 0; paramIdx < m_pDescr->controllerSection->parameters.size(); ++paramIdx){
            const auto& nrpn = m_pDescr->controllerSection->parameters[paramIdx].midi.nrpn;
            if(nrpn &&
            nrpn->operator[](0) == nrpnMsg.idMsb &&
            nrpn->operator[](1) == nrpnMsg.idLsb)
            {
                for(auto& cb : m_controlParameterCbs) cb(nrpnMsg.channelNr - 1, paramIdx, value);
                return;
            }
        }
    }
}

void MidiInMsgHandler::handleIncomingNoteOnMsg(const midi::Message<midi::NoteOn>& noteOnMsg) noexcept
{

}

void MidiInMsgHandler::handleIncomingNoteOffMsg(const midi::Message<midi::NoteOff>& noteOffMsg) noexcept
{

}

void MidiInMsgHandler::registerForSoundParameter(Cb cb) noexcept
{
    m_soundParameterCbs.push_back(cb);
}

void MidiInMsgHandler::registerForControlParameter(Cb cb) noexcept
{
    m_controlParameterCbs.push_back(cb);
}

void MidiInMsgHandler::recvParameter(midi::Message<midi::ControlChange> ccMsg) noexcept
{
    const auto channelId = ccMsg.channel() - 1;
    const auto parameterId = m_cc2IdCache[ccMsg.controllerNumber()].parameterId;
    const auto type = m_cc2IdCache[ccMsg.controllerNumber()].deviceType;
    if(SoundDevParameterId::NONE == parameterId)
    {
        return;
    }
    const float value = ccMsg.controllerValue() / 128.0f;
    switch(type)
    {
        case MusicDeviceDescription::Type::SoundDevice:
        {
            for(auto& cb : m_soundParameterCbs) cb(channelId, parameterId, value);
            break;
        }
        case MusicDeviceDescription::Type::ControllerDevice:
        {
            for(auto& cb : m_controlParameterCbs) cb(channelId, parameterId, value);
            break;
        }
        default: break;
    }
}

void MidiInMsgHandler::recvParameter(const midi::Message<midi::ControlChange>& ccMsgMsb,
                                     const midi::Message<midi::ControlChange>& ccMsgLsb) noexcept
{
    const auto channelId = ccMsgMsb.channel() - 1;
    const auto parameterId = m_cc2IdCache[ccMsgMsb.controllerNumber()].parameterId;
    const auto type = m_cc2IdCache[ccMsgMsb.controllerNumber()].deviceType;
    const float value = ((ccMsgMsb.controllerValue() << 7) + ccMsgLsb.controllerValue())  / float(128 * 128);
    switch(type)
    {
        case MusicDeviceDescription::Type::SoundDevice:
        {
            for(auto& cb : m_soundParameterCbs) cb(channelId, parameterId, value);
            break;
        }
        case MusicDeviceDescription::Type::ControllerDevice:
        {
            for(auto& cb : m_controlParameterCbs) cb(channelId, parameterId, value);
            break;
        }
        default: break;
    }
}

bool MidiInMsgHandler::isCcMsb(midi::Message<midi::ControlChange> ccMsg) const noexcept
{
    const auto parameterId = m_cc2IdCache[ccMsg.controllerNumber()].parameterId;
    return (ccMsg.controllerNumber() == m_pDescr->soundSection->parameters[parameterId].midi.cc[0]);   
}