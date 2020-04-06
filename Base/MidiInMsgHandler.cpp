#include "MidiInMsgHandler.h"

MidiInMsgHandler::MidiInMsgHandler(
   std::unique_ptr<midi::IMidiInMedium> pMedium,
   std::shared_ptr<MusicDeviceDescription> pDescr) :
   m_midiIn(std::move(pMedium)),
   m_pDescr(pDescr)
{
   initMappingCaches();

   m_midiIn.registerMidiInCb([this](const midi::MidiMessage& midiMsg) {
      auto iter = m_map.find(MapKey(midiMsg));
      if(m_map.end() == iter) return;

      mpark::visit( midi::overload{
         [this, &midiMsg](const SoundDevParameterId& id){
            handleSoundDevParameterRouting(id, midiMsg);
         },
         [this, &midiMsg](const ControllerEventId& id){
            handleControllerParameterRouting(id, midiMsg);
         },
         [](auto&& other) { /* Ignore them */ }
      }, *iter);
   });
}

void MidiInMsgHandler::handleSoundDevParameterRouting(const SoundDevParameterId& id, 
                                                      const midi::MidiMessage& midiMsg) const noexcept
{
   mpark::visit( midi::overload{
      [this, &id](const midi::Message<midi::ControlChange>& msg) {
         for(auto& cb : m_soundParameterCbs) cb(msg.channel() - 1, id.parameterId, msg.getRelativeValue());
      },
      [this, &id](const midi::Message<midi::ControlChangeHighRes>& msg) {
         for(auto& cb : m_soundParameterCbs) cb(msg.channel() - 1, id.parameterId, msg.getRelativeValue());
      },
      [this, &id](const midi::Message<midi::NRPN>& msg) {
         for(auto& cb : m_soundParameterCbs) cb(msg.channel() - 1, id.parameterId, msg.getRelativeValue());  
      },
      [this, &id](const midi::Message<midi::RPN>& msg) {
         for(auto& cb : m_soundParameterCbs) cb(msg.channel() - 1, id.parameterId, msg.getRelativeValue());  
      },
      [](auto&& other) { /* Ignore them */ }
   }, midiMsg);
}

void MidiInMsgHandler::handleControllerParameterRouting(const SoundDevParameterId& id, 
                                                        const midi::MidiMessage& midiMsg) const noexcept
{
   if(!m_pDescr->controllerSection) return;
   const auto& eventDescr = m_pDescr->controllerSection->widgets[id.widgetId.id].events[id.eventId];
   const EventType::Value value = 
   mpark::visit( midi::overload{
      [this, &id, &eventDescr](const midi::Message<midi::ControlChange>& msg) -> EventType
      {
         return mpark::visit( midi::overload{
            [this, id, &msg](const WidgetParamType::Button& evt) -> EventType{
               return PressReleaseEvent{id, bool(msg.controllerValue())};
            },
            [this, id, &msg](const WidgetParamType::Continous& evt) -> EventType{
               return ContinousValueEvent{id, msg.getRelativeValue()};
            },
            [this, id, &msg](const WidgetParamType::Incremental& evt) -> EventType{
               return IncrementEvent{id, msg.controllerValue()};
            }
         }, eventDescr);
      },
      [this, &id, &eventDescr](const midi::Message<midi::ControlChangeHighRes>& msg) 
      {
         return mpark::visit( midi::overload{
            [this, id, &msg](const WidgetParamType::Button& evt) -> EventType{
               return PressReleaseEvent{id, bool(msg.controllerValue())};
            },
            [this, id, &msg](const WidgetParamType::Continous& evt) -> EventType{
               return ContinousValueEvent{id, msg.getRelativeValue()};
            },
            [this, id, &msg](const WidgetParamType::Incremental& evt) -> EventType{
               return IncrementEvent{id, msg.controllerValue()};
            }
         }, eventDescr);
      },
      [this, &id, &eventDescr](const midi::Message<midi::NRPN>& msg) 
      {
         return mpark::visit( midi::overload{
            [this, id, &msg](const WidgetParamType::Button& evt) -> EventType{
               return PressReleaseEvent{id, bool(msg.controllerValue())};
            },
            [this, id, &msg](const WidgetParamType::Continous& evt) -> EventType{
               return ContinousValueEvent{id, msg.getRelativeValue()};
            },
            [this, id, &msg](const WidgetParamType::Incremental& evt) -> EventType{
               return IncrementEvent{id, msg.controllerValue()};
            }
         }, eventDescr);
      },
      [this, &id, &eventDescr](const midi::Message<midi::NoteOn>& msg) 
      {
         return mpark::visit( midi::overload{
            [this, id, &msg](const WidgetParamType::Button& evt) -> EventType{
               return PressReleaseEvent{id, bool(msg.controllerValue())};
            },
            [this, id, &msg](const WidgetParamType::Continous& evt) -> EventType{
               return ContinousValueEvent{id, msg.getRelativeValue()};
            },
            [this, id, &msg](const WidgetParamType::Incremental& evt) -> EventType{
               return IncrementEvent{id, msg.controllerValue()};
            }
         }, eventDescr);
      },
      [this, &id, &eventDescr](const midi::Message<midi::NoteOff>& msg) 
      {
         return mpark::visit( midi::overload{
            [this, id, &msg](const WidgetParamType::Button& evt) -> EventType{
               return PressReleaseEvent{id, bool(msg.controllerValue())};
            },
            [this, id, &msg](const WidgetParamType::Continous& evt) -> EventType{
               return ContinousValueEvent{id, msg.getRelativeValue()};
            },
            [this, id, &msg](const WidgetParamType::Incremental& evt) -> EventType{
               return IncrementEvent{id, msg.controllerValue()};
            }
         }, eventDescr);
      },
      [this, &id, &eventDescr](const midi::Message<midi::AfterTouchChannel>& msg) 
      {
         return mpark::visit( midi::overload{
            [this, id, &msg](const WidgetParamType::Button& evt) -> EventType{
               return PressReleaseEvent{id, bool(msg.controllerValue())};
            },
            [this, id, &msg](const WidgetParamType::Continous& evt) -> EventType{
               return ContinousValueEvent{id, msg.getRelativeValue()};
            },
            [this, id, &msg](const WidgetParamType::Incremental& evt) -> EventType{
               return IncrementEvent{id, msg.controllerValue()};
            }
         }, eventDescr);
      },
      [this, &id, &eventDescr](const midi::Message<midi::AfterTouchPoly>& msg) 
      {
         return mpark::visit( midi::overload{
            [this, id, &msg](const WidgetParamType::Button& evt) -> EventType{
               return PressReleaseEvent{id, bool(msg.controllerValue())};
            },
            [this, id, &msg](const WidgetParamType::Continous& evt) -> EventType{
               return ContinousValueEvent{id, msg.getRelativeValue()};
            },
            [this, id, &msg](const WidgetParamType::Incremental& evt) -> EventType{
               return IncrementEvent{id, msg.controllerValue()};
            }
         }, eventDescr);
      },
      [](auto&& other) { /* Ignore them */ }
   }, midiMsg);
   m_contrEvtConsumer.fireCtrlEvent(EventType{ value});
}

void MidiInMsgHandler::initMappingCaches() noexcept
{
   if(m_pDescr->soundSection)
   {
      for(auto& param : m_pDescr->soundSection->parameters)
      {
         if(param.midi.cc.size() == 2)
         {
            m_midiIn.setCCHighResPair(param.midi.cc[0], param.midi.cc[1]);
         }
      }
   }

   if(m_pDescr->controllerSection)
   {
      // TODO
   }
}

void MidiInMsgHandler::handleIncomingCcMsg(
   const midi::Message<midi::ControlChange>& ccMsg) noexcept
{
   const auto& entry = m_cc2IdCache[ccMsg.controllerNumber()];
   if (!entry)
      return;

   mpark::visit(midi::overload{[this, &ccMsg](const SoundDevParameterId& id) {
                                  for (auto& cb : m_soundParameterCbs)
                                     cb(ccMsg.channel() - 1, id.parameterId,
                                        ccMsg.getRelativeValue());
                               },
                               [this](const ControllerEventId& id) {

                               }},
                *entry);
}

void MidiInMsgHandler::handleIncomingCcHighResMsg(
   const midi::Message<midi::ControlChangeHighRes>& ccHrMsg) noexcept
{
}

void MidiInMsgHandler::handleIncomingNRPNMsg(
   const midi::Message<midi::NRPN>& nrpnMsg) noexcept
{
   const float value = nrpnMsg.getRelativeValue();
   if (m_pDescr->soundSection)
   {
      for (int paramIdx = 0;
           paramIdx < m_pDescr->soundSection->parameters.size(); ++paramIdx)
      {
         const auto& nrpn =
            m_pDescr->soundSection->parameters[paramIdx].midi.nrpn;
         if (nrpn && nrpn->operator[](0) == nrpnMsg.idMsb &&
             nrpn->operator[](1) == nrpnMsg.idLsb)
         {
            for (auto& cb : m_soundParameterCbs)
               cb(nrpnMsg.channelNr - 1, paramIdx, value);
            return;
         }
      }
   }
   if (m_pDescr->controllerSection)
   {
      for (int paramIdx = 0;
           paramIdx < m_pDescr->controllerSection->parameters.size();
           ++paramIdx)
      {
         const auto& nrpn =
            m_pDescr->controllerSection->parameters[paramIdx].midi.nrpn;
         if (nrpn && nrpn->operator[](0) == nrpnMsg.idMsb &&
             nrpn->operator[](1) == nrpnMsg.idLsb)
         {
            for (auto& cb : m_controlParameterCbs)
               cb(nrpnMsg.channelNr - 1, paramIdx, value);
            return;
         }
      }
   }
}

void MidiInMsgHandler::handleIncomingNoteOnMsg(
   const midi::Message<midi::NoteOn>& noteOnMsg) noexcept
{
}

void MidiInMsgHandler::handleIncomingNoteOffMsg(
   const midi::Message<midi::NoteOff>& noteOffMsg) noexcept
{
}

void MidiInMsgHandler::handleIncomingAfterTouchChannelMsg(
   const midi::Message<midi::AfterTouchChannel>& msg) noexcept
{
}

void MidiInMsgHandler::handleIncomingAfterTouchPolyMsg(
   const midi::Message<midi::AfterTouchPoly>& msg) noexcept
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

void MidiInMsgHandler::recvParameter(
   midi::Message<midi::ControlChange> ccMsg) noexcept
{
   const auto channelId   = ccMsg.channel() - 1;
   const auto parameterId = m_cc2IdCache[ccMsg.controllerNumber()].parameterId;
   const auto type        = m_cc2IdCache[ccMsg.controllerNumber()].deviceType;
   if (SoundDevParameterId::NONE == parameterId)
   {
      return;
   }
   const float value = ccMsg.controllerValue() / 128.0f;
   switch (type)
   {
      case MusicDeviceDescription::Type::SoundDevice:
      {
         for (auto& cb : m_soundParameterCbs) cb(channelId, parameterId, value);
         break;
      }
      case MusicDeviceDescription::Type::ControllerDevice:
      {
         for (auto& cb : m_controlParameterCbs)
            cb(channelId, parameterId, value);
         break;
      }
      default: break;
   }
}

void MidiInMsgHandler::recvParameter(
   const midi::Message<midi::ControlChange>& ccMsgMsb,
   const midi::Message<midi::ControlChange>& ccMsgLsb) noexcept
{
   const auto channelId = ccMsgMsb.channel() - 1;
   const auto parameterId =
      m_cc2IdCache[ccMsgMsb.controllerNumber()].parameterId;
   const auto type = m_cc2IdCache[ccMsgMsb.controllerNumber()].deviceType;
   const float value =
      ((ccMsgMsb.controllerValue() << 7) + ccMsgLsb.controllerValue()) /
      float(128 * 128);
   switch (type)
   {
      case MusicDeviceDescription::Type::SoundDevice:
      {
         for (auto& cb : m_soundParameterCbs) cb(channelId, parameterId, value);
         break;
      }
      case MusicDeviceDescription::Type::ControllerDevice:
      {
         for (auto& cb : m_controlParameterCbs)
            cb(channelId, parameterId, value);
         break;
      }
      default: break;
   }
}

bool MidiInMsgHandler::isCcMsb(midi::Message<midi::ControlChange> ccMsg) const
   noexcept
{
   const auto parameterId = m_cc2IdCache[ccMsg.controllerNumber()].parameterId;
   return (ccMsg.controllerNumber() ==
           m_pDescr->soundSection->parameters[parameterId].midi.cc[0]);
}