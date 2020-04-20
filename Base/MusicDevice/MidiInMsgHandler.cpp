#include "MidiInMsgHandler.h"

#include <loguru.hpp>
#include <mpark/variant.hpp>

#include "JsonCast.h"

std::string base::MidiInMsgHandler::cache2Str(
   const std::unordered_map<MidiMessageId, MapDest>& map)
{
   std::string ret;
   for (auto iter = map.begin(); iter != map.end(); iter++)
   {
      ret.append(meta::serialize(iter->first).dump() + " --> " +
                 meta::serialize(iter->second).dump() + "\n");
   }
   return ret;
}

base::MidiInMsgHandler::MidiInMsgHandler(
   std::unique_ptr<midi::IMidiInMedium> pMedium,
   std::shared_ptr<MusicDeviceDescription> pDescr) :
   m_midiIn(std::move(pMedium)),
   m_pDescr(pDescr)
{
   initCacheBySoundSection();
   LOG_F(INFO, "Initialized Controller cache \n{}", cache2Str(m_map));
   initCacheByControllerSection();

   m_midiIn.registerMidiInCb([this](const midi::MidiMessage& midiMsg) {
      const auto midiId = midiMessageToId(midiMsg);
      auto iter         = m_map.find(midiId);
      if (m_map.end() == iter)
      {
         LOG_F(ERROR, "No mapping for Midi msg id {}",
               meta::serialize(midiId).dump());
         return;
      }
      mpark::visit(
         midi::overload{[this, &midiMsg](const SoundDevParameterId& id) {
                           handleSoundDevParameterRouting(id, midiMsg);
                        },
                        [this, &midiMsg](const ctrldev::EventId& id) {
                           handleControllerParameterRouting(id, midiMsg);
                        }},
         iter->second);
   });
}

void base::MidiInMsgHandler::handleSoundDevParameterRouting(
   const SoundDevParameterId& id, const midi::MidiMessage& midiMsg) const
   noexcept
{
   mpark::visit(
      midi::overload{
         [this, &id](const midi::Message<midi::ControlChange>& msg) {
            for (auto& cb : m_soundParameterCbs)
               cb(msg.channel() - 1, id.parameterId, msg.getRelativeValue());
         },
         [this, &id](const midi::Message<midi::ControlChangeHighRes>& msg) {
            for (auto& cb : m_soundParameterCbs)
               cb(msg.channel() - 1, id.parameterId, msg.getRelativeValue());
         },
         [this, &id](const midi::Message<midi::NRPN>& msg) {
            for (auto& cb : m_soundParameterCbs)
               cb(msg.channel() - 1, id.parameterId, msg.getRelativeValue());
         },
         [this, &id](const midi::Message<midi::RPN>& msg) {
            for (auto& cb : m_soundParameterCbs)
               cb(msg.channel() - 1, id.parameterId, msg.getRelativeValue());
         },
         [](auto&& other) { /* Ignore them */ }},
      midiMsg);
}

void base::MidiInMsgHandler::handleControllerParameterRouting(
   ctrldev::EventId id, const midi::MidiMessage& midiMsg) const noexcept
{
   if (!m_pDescr->controllerSection)
      return;
   const auto& eventDescr =
      m_pDescr->controllerSection->widgets[id.widgetId].events[id.eventId];

   const bool mpeMode = (m_pDescr->controllerSection->widgets[id.widgetId].mpe &&
                         *m_pDescr->controllerSection->widgets[id.widgetId].mpe);

   const auto value = mpark::visit(
      midi::overload{
         [this, &id, &eventDescr](const midi::Message<midi::ControlChange>& msg)
            -> ctrldev::EventValue {
            if (!id.widgetCoord)
               id.widgetCoord = m_mpeMap[msg.channel() - 1];
            return mpark::visit(
               midi::overload{
                  [this, &msg](const ControllerDeviceEventPressRelease& evt)
                     -> ctrldev::EventValue {
                        if(evt.sourceHasInvertedLogic && *evt.sourceHasInvertedLogic){
                           return ctrldev::PressReleaseType{
                              msg.controllerValue() ? -1.0f : 1.0f};
                        }
                        else{
                           return ctrldev::PressReleaseType{
                              msg.controllerValue() ? 1.0f : -1.0f};
                        }
                  },
                  [this, &msg](const ControllerDeviceEventContinousValue& evt)
                     -> ctrldev::EventValue {
                     return ctrldev::ContinousValueType{msg.getRelativeValue()};
                  },
                  [this, &msg](const ControllerDeviceEventIncremental& evt)
                     -> ctrldev::EventValue {
                     return ctrldev::IncrementType{
                        msg.controllerValue() -
                        (midi::Message<midi::ControlChange>::RES_MAX / 2)};
                  }},
               eventDescr);
         },
         [this, &id,
          &eventDescr](const midi::Message<midi::ControlChangeHighRes>& msg)
            -> ctrldev::EventValue {
            if (!id.widgetCoord)
               id.widgetCoord = m_mpeMap[msg.channel() - 1];
            return mpark::visit(
               midi::overload{
                  [this, &msg](const ControllerDeviceEventPressRelease& evt)
                     -> ctrldev::EventValue {
                     return ctrldev::PressReleaseType{
                        msg.controllerValue() ? 1.0f : -1.0f};
                  },
                  [this, &msg](const ControllerDeviceEventContinousValue& evt)
                     -> ctrldev::EventValue {
                     return ctrldev::ContinousValueType{msg.getRelativeValue()};
                  },
                  [this, &msg](const ControllerDeviceEventIncremental& evt)
                     -> ctrldev::EventValue {
                     return ctrldev::IncrementType{
                        msg.controllerValue() -
                        (midi::Message<midi::ControlChangeHighRes>::RES_MAX /
                         2)};
                  }},
               eventDescr);
         },
         [this, &id, &eventDescr](
            const midi::Message<midi::NRPN>& msg) -> ctrldev::EventValue {
            if (!id.widgetCoord)
               id.widgetCoord = m_mpeMap[msg.channel() - 1];
            return mpark::visit(
               midi::overload{
                  [this, &msg](const ControllerDeviceEventPressRelease& evt)
                     -> ctrldev::EventValue {
                     return ctrldev::PressReleaseType{msg.getValue() ? 1.0f
                                                                     : -1.0f};
                  },
                  [this, &msg](const ControllerDeviceEventContinousValue& evt)
                     -> ctrldev::EventValue {
                     return ctrldev::ContinousValueType{msg.getRelativeValue()};
                  },
                  [this, &msg](const ControllerDeviceEventIncremental& evt)
                     -> ctrldev::EventValue {
                     return ctrldev::IncrementType{
                        msg.getValue() -
                        (midi::Message<midi::NRPN>::RES_MAX / 2)};
                  }},
               eventDescr);
         },
         [this, &id, &eventDescr, mpeMode](
            const midi::Message<midi::NoteOn>& msg) -> ctrldev::EventValue {
            if (mpeMode)
            {
               if (!id.widgetCoord)
               {
                  LOG_F(ERROR, "MPE mode note-on without widget coordinate");
                  return mpark::monostate();
               }
               m_mpeMap[msg.channel() - 1] = id.widgetCoord;
            }
            return mpark::visit(
               midi::overload{
                  [this, &msg](const ControllerDeviceEventPressRelease& evt)
                     -> ctrldev::EventValue {
                     return ctrldev::PressReleaseType{msg.relativeVelocity()};
                  },
                  [](auto &&) -> ctrldev::EventValue {
                     return mpark::monostate();
                  }},
               eventDescr);
         },
         [this, &eventDescr](
            const midi::Message<midi::NoteOff>& msg) -> ctrldev::EventValue {
            return mpark::visit(
               midi::overload{
                  [this, &msg](const ControllerDeviceEventPressRelease& evt)
                     -> ctrldev::EventValue {
                     return ctrldev::PressReleaseType{-1.0f *
                                                      msg.relativeVelocity()};
                  },
                  [](auto &&) -> ctrldev::EventValue {
                     return mpark::monostate();
                  }},
               eventDescr);
         },
         [this, &id,
          &eventDescr](const midi::Message<midi::AfterTouchChannel>& msg)
            -> ctrldev::EventValue {
            if (!id.widgetCoord)
               id.widgetCoord = m_mpeMap[msg.channel() - 1];
            return mpark::visit(
               midi::overload{
                  [this, &msg](const ControllerDeviceEventContinousValue& evt)
                     -> ctrldev::EventValue {
                     return ctrldev::ContinousValueType{msg.relativeValue()};
                  },
                  [](auto &&) -> ctrldev::EventValue {
                     return mpark::monostate();
                  }},
               eventDescr);
         },
         [this, &eventDescr](const midi::Message<midi::AfterTouchPoly>& msg)
            -> ctrldev::EventValue {
            return mpark::visit(
               midi::overload{
                  [this, &msg](const ControllerDeviceEventContinousValue& evt)
                     -> ctrldev::EventValue {
                     return ctrldev::ContinousValueType{msg.relativePressure()};
                  },
                  [](auto &&) -> ctrldev::EventValue {
                     return mpark::monostate();
                  }},
               eventDescr);
         },
         [this, &id, &eventDescr](
            const midi::Message<midi::PitchBend>& msg) -> ctrldev::EventValue {
            if (!id.widgetCoord)
               id.widgetCoord = m_mpeMap[msg.channel() - 1];
            return mpark::visit(
               midi::overload{
                  [this, &msg](const ControllerDeviceEventContinousValue& evt)
                     -> ctrldev::EventValue {
                     return ctrldev::ContinousValueType{
                        float(msg.value())}; // TODO: is value() correct?
                  },
                  [](auto &&) -> ctrldev::EventValue {
                     return mpark::monostate();
                  }},
               eventDescr);
         },
         [](auto&& other) -> ctrldev::EventValue {
            return mpark::monostate();
         }},
      midiMsg);
   if (!mpark::holds_alternative<mpark::monostate>(value))
   {
      ctrldev::Event e{id, value};
      LOG_F(INFO, "Processed: {}", meta::serialize(e).dump());
      // m_contrEvtConsumer.fireCtrlEvent(EventType{id, value}); TODO
   }
}

void base::MidiInMsgHandler::handleEventSource(
   const std::vector<std::vector<MidiMessageId>>& source, int widgetId,
   int eventId) noexcept
{
   assert(source.size() > 0 && source[0].size() > 0);
   if (source.size() > 1 || source[0].size() > 1)
   {
      for (int row = 0; row < source.size(); ++row)
      {
         for (int col = 0; col < source[row].size(); ++col)
         {
            m_map[source[row][col]] = ctrldev::EventId{
               widgetId, std::optional<ctrldev::WidgetCoord>({row, col}),
               eventId};
         }
      }
   }
   else
   {
      m_map[source[0][0]] = ctrldev::EventId{widgetId, std::nullopt, eventId};
   }
}

void base::MidiInMsgHandler::initCacheBySoundSection() noexcept
{
   if (!m_pDescr->soundSection)
   {
      return;
   }

   for (const auto& param : m_pDescr->soundSection->parameters)
   {
      if (param.midi.cc.size() == 2)
      {
         m_midiIn.setCCHighResPair(param.midi.cc[0], param.midi.cc[1]);
      }
   }

   // m_map[]
   // TODO
   // TODO
   // TODO
   // TODO


}

void base::MidiInMsgHandler::initCacheByControllerSection() noexcept
{
   if (!m_pDescr->controllerSection)
   {
      return;
   }

   for (int widgetId = 0;
        widgetId < m_pDescr->controllerSection->widgets.size(); ++widgetId)
   {
      const auto& widget = m_pDescr->controllerSection->widgets[widgetId];
      for (int eventId = 0; eventId < widget.events.size(); ++eventId)
      {
         const auto& event = widget.events[eventId];
         mpark::visit(
            midi::overload{
               [this, widgetId,
                eventId](const ControllerDeviceEventPressRelease& evt) {
                  handleEventSource(evt.pressSource, widgetId, eventId);
                  handleEventSource(evt.releaseSource, widgetId, eventId);
               },
               [this, widgetId,
                eventId](const ControllerDeviceEventContinousValue& evt) {
                  handleEventSource(evt.source, widgetId, eventId);
               },
               [this, widgetId,
                eventId](const ControllerDeviceEventIncremental& evt) {
                  handleEventSource(evt.source, widgetId, eventId);
               }},
            event);
      }
   }
}

void base::MidiInMsgHandler::registerForSoundParameter(Cb cb) noexcept
{
   m_soundParameterCbs.push_back(cb);
}

void base::MidiInMsgHandler::registerForControlParameter(Cb cb) noexcept
{
   m_controlParameterCbs.push_back(cb);
}

void base::MidiInMsgHandler::processMidiInBuffers() { m_midiIn.update(); }
