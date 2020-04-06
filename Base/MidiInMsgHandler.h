#ifndef MIDI_IN_MSG_HANDLER_H
#define MIDI_IN_MSG_HANDLER_H

#include <array>
#include <cstdint>
#include <memory>
#include <mpark/variant.hpp>
#include <optional>
#include <unordered_map>
#include <vector>

#include "ControllerDeviceEvents.h"
#include "Midi.h"
#include "Midi1Input.h"
#include "MusicDeviceDescription.h"

namespace midi
{
class Router;
}
namespace midi
{
class Dumper;
}

class MidiInMsgHandler
{
public:
   MidiInMsgHandler(std::unique_ptr<midi::IMidiInMedium> pMedium,
                    std::shared_ptr<MusicDeviceDescription> pDescr);

   using Cb =
      std::function<void(uint32_t voiceId, uint32_t parameterId, float value)>;
   void registerForSoundParameter(Cb cb) noexcept;
   void registerForControlParameter(Cb cb) noexcept;

   friend midi::Router;
   friend midi::Dumper;

private:
   void initMappingCaches() noexcept;

   void handleSoundDevParameterRouting(const SoundDevParameterId& id, 
                                       const midi::MidiMessage& midiMsg) const noexcept;
   void handleControllerParameterRouting(const SoundDevParameterId& id, 
                                         const midi::MidiMessage& midiMsg) const noexcept;


   void handleIncomingCcMsg(
      const midi::Message<midi::ControlChange>& msg) noexcept;
   void handleIncomingCcHighResMsg(
      const midi::Message<midi::ControlChangeHighRes>& msg) noexcept;
   void handleIncomingNRPNMsg(const midi::Message<midi::NRPN>& msg) noexcept;
   void handleIncomingNoteOnMsg(
      const midi::Message<midi::NoteOn>& msg) noexcept;
   void handleIncomingNoteOffMsg(
      const midi::Message<midi::NoteOff>& msg) noexcept;
   void handleIncomingAfterTouchChannelMsg(
      const midi::Message<midi::AfterTouchChannel>& msg) noexcept;
   void handleIncomingAfterTouchPolyMsg(
      const midi::Message<midi::AfterTouchPoly>& msg) noexcept;

   midi::Midi1Input m_midiIn;
   std::shared_ptr<MusicDeviceDescription> m_pDescr;

   // -------------------------
   // Main Map
   // -------------------------

   // Map keys
   struct MapKey
   {
      MapKey(const midi::MidiMessage& midiMsg)
      {
         mpark::visit(
            midi::overload{
               [this](const midi::Message<midi::ControlChange>& msg) {
                  midiMsgType = midi::ControlChange;
                  id = msg.controllerNumber();
               },
               [this](const midi::Message<midi::ControlChangeHighRes>& msg) {
                  midiMsgType = midi::ControlChangeHighRes;
                  id = msg.controllerNumber();
               },
               [this](const midi::Message<midi::NRPN>& msg) {
                  midiMsgType = midi::NRPN;
                  id = (msg.idMsb << 7) ^ msg.idLsb;
               },
               [this](const midi::Message<midi::RPN>& msg) {
                  midiMsgType = midi::RPN;
                  id = (msg.idMsb << 7) ^ msg.idLsb;
               },
               [this](const midi::Message<midi::NoteOn>& msg) {
                  midiMsgType = midi::NoteOn;
                  id = msg.noteNumber();
               },
               [this](const midi::Message<midi::NoteOff>& msg) {
                  midiMsgType = midi::NoteOff;
                  id = msg.noteNumber();
               },
               [this](const midi::Message<midi::AfterTouchChannel>& msg) {
                  midiMsgType = midi::AfterTouchChannel;
                  id = msg.value();
               },
               [this](const midi::Message<midi::AfterTouchPoly>& msg) {
                  midiMsgType = midi::AfterTouchPoly;
                  id = msg.noteNumber();
               },
               [](auto&& other) { /* Ignore them */ }},
            midiMsg);
      }

      midi::MsgType midiMsgType{midi::NoValidMsg};
      uint16_t id{0};
      struct HashFn
      {
         size_t operator()(const MapKey& mapKey) const noexcept
         {
            return (static_cast<uint16_t>(mapKey.midiMsgType)
                    << (sizeof(mapKey.id) * 8)) ^
                   mapKey.id;
         }
      };
   };

   // Map Values
   static constexpr int INVALID_IDX = -1;
   struct SoundDevParameterId
   {
      int parameterId{INVALID_IDX};
   };
   struct ControllerEventId
   {
      base::ctrldev::WidgetId widgetId;
      int eventId{INVALID_IDX};
   };

   using MapDest = mpark::variant<SoundDevParameterId, ControllerEventId>;

   std::unordered_map<MapKey, MapDest, MapKey::HashFn> m_map;

   // -------------------------
   // MPE Map
   // -------------------------
   std::array<base::ctrldev::WidgetCoord, midi::NUM_CHANNLES> m_mpeMap;

   void recvParameter(midi::Message<midi::ControlChange> msg) noexcept;
   void recvParameter(
      const midi::Message<midi::ControlChange>& ccMsgMsb,
      const midi::Message<midi::ControlChange>& ccMsgLsb) noexcept;
   bool isCcMsb(midi::Message<midi::ControlChange> ccMsg) const noexcept;

   std::vector<Cb> m_soundParameterCbs;
   std::vector<Cb> m_controlParameterCbs;
};

#endif