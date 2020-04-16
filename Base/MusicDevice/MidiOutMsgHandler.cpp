#include "MidiOutMsgHandler.h"

#include <loguru.hpp>

MidiOutMsgHandler::MidiOutMsgHandler(
   std::unique_ptr<midi::IMidiOutMedium> pMedium,
   std::shared_ptr<MusicDeviceDescription> pDescr) noexcept :
   m_midiOut(std::move(pMedium)),
   m_pDescr(pDescr)
{
   if (m_pDescr->soundSection && m_pDescr->soundSection->pitchBendFactor)
   {
      m_pitchBendFactor = *m_pDescr->soundSection->pitchBendFactor;
   }
}

void MidiOutMsgHandler::sendSoundParameter(uint32_t voiceId,
                                           uint32_t parameterId,
                                           float value) noexcept
{
   if (!m_pDescr->soundSection)
      return;
   const auto& nrpn = m_pDescr->soundSection->parameters[parameterId].midi.nrpn;
   if (nrpn)
   {
      m_midiOut.send(
         midi::Message<midi::NRPN>(voiceId + 1, (*nrpn)[0], (*nrpn)[1], value));
   }
   else
   {
      auto& cc = m_pDescr->soundSection->parameters[parameterId].midi.cc;
      switch (cc.size())
      {
         case 1:
         {
            m_midiOut.send(
               midi::Message<midi::ControlChange>(voiceId + 1, cc[0], value));
            break;
         }
         case 2:
         {
            m_midiOut.send(
               midi::Message<midi::ControlChangeHighRes>(voiceId + 1, cc[0], cc[1], value));
            break;
         }
         default:
         {
            LOG_F(ERROR, "bad cc-vector size in config");
            break;
         }
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
   if (m_pDescr->soundSection && m_pDescr->soundSection->parameter_dump_request)
   {
      if (m_pDescr->soundSection->parameter_dump_request->midi_sysex)
      {
         m_midiOut.sysEx(
            *m_pDescr->soundSection->parameter_dump_request->midi_sysex);
      }
      if (m_pDescr->soundSection->parameter_dump_request->midi_cc)
      {
         m_midiOut.controlParameter(
            1, m_pDescr->soundSection->parameter_dump_request->midi_cc->cc[0],
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

void MidiOutMsgHandler::noteOn(int voiceIndex, int note,
                               float velocity) noexcept
{
   m_midiOut.noteOn(voiceIndex + 1, note, velocity * 127.0);
}

void MidiOutMsgHandler::noteOff(int voiceIndex, int note,
                                float velocity) noexcept
{
   m_midiOut.noteOff(voiceIndex + 1, note, velocity * 127.0);
}

void MidiOutMsgHandler::pitchBend(int voiceIndex, float value) noexcept
{
   m_midiOut.pitchBend(voiceIndex + 1, value * m_pitchBendFactor * 16383);
}