#ifndef MIDI_MESSAGE_IDS_EXTENSION_H
#define MIDI_MESSAGE_IDS_EXTENSION_H

#include "MidiMessageIds.h"

inline bool operator==(const MidiMsgId<midi::ControlChange>& lhs,
                       const MidiMsgId<midi::ControlChange>& rhs) noexcept
{
   return lhs.id == rhs.id;
}

inline bool operator==(const MidiMsgId<midi::ControlChangeHighRes>& lhs,
                       const MidiMsgId<midi::ControlChangeHighRes>& rhs) noexcept
{
   return (lhs.idMsb == rhs.idMsb) && (lhs.idLsb == rhs.idLsb);
}

inline bool operator==(const MidiMsgId<midi::NRPN>& lhs,
                       const MidiMsgId<midi::NRPN>& rhs) noexcept
{
   return (lhs.idMsb == rhs.idMsb) && (lhs.idLsb == rhs.idLsb);
}

inline bool operator==(const MidiMsgId<midi::RPN>& lhs,
                       const MidiMsgId<midi::RPN>& rhs) noexcept
{
   return (lhs.idMsb == rhs.idMsb) && (lhs.idLsb == rhs.idLsb);
}

inline bool operator==(const MidiMsgId<midi::NoteOn>& lhs,
                       const MidiMsgId<midi::NoteOn>& rhs) noexcept
{
   return lhs.note == rhs.note;
}

inline bool operator==(const MidiMsgId<midi::NoteOff>& lhs,
                       const MidiMsgId<midi::NoteOff>& rhs) noexcept
{
   return lhs.note == rhs.note;
}

inline bool operator==(const MidiMsgId<midi::AfterTouchChannel>& lhs,
                       const MidiMsgId<midi::AfterTouchChannel>& rhs) noexcept
{
   return true;
}

inline bool operator==(const MidiMsgId<midi::AfterTouchPoly>& lhs,
                       const MidiMsgId<midi::AfterTouchPoly>& rhs) noexcept
{
   return lhs.note == rhs.note;
}

inline bool operator==(const MidiMsgId<midi::PitchBend>& lhs,
                       const MidiMsgId<midi::PitchBend>& rhs) noexcept
{
   return true;
}


namespace std
{
template<>
struct hash<MidiMessageId>
{
   size_t operator()(const MidiMessageId& midiMessageId) const noexcept
   {
      hash<int> hasher;
      const auto index = midiMessageId.index();
      return mpark::visit(
         midi::overload{
            [index](const MidiMsgId<midi::ControlChange>& msg) -> size_t {
               return (index << 16) ^ msg.id;
            },
            [index](const MidiMsgId<midi::ControlChangeHighRes>& msg)
               -> size_t { return (index << 16) ^ msg.idMsb; },
            [index](const MidiMsgId<midi::NRPN>& msg) -> size_t {
               return (index << 16) ^ (msg.idMsb << 7) ^ msg.idLsb;
            },
            [index](const MidiMsgId<midi::RPN>& msg) -> size_t {
               return (index << 16) ^ (msg.idMsb << 7) ^ msg.idLsb;
            },
            [index](const MidiMsgId<midi::NoteOn>& msg) -> size_t {
               return (index << 16) ^ msg.note;
            },
            [index](const MidiMsgId<midi::NoteOff>& msg) -> size_t {
               return (index << 16) ^ msg.note;
            },
            [index](const MidiMsgId<midi::AfterTouchChannel>& msg) -> size_t {
               return (index << 16);
            },
            [index](const MidiMsgId<midi::AfterTouchPoly>& msg) -> size_t {
               return (index << 16) + msg.note;
            },
            [index](const MidiMsgId<midi::PitchBend>& msg) -> size_t {
               return (index << 16);
            },
            [](auto&& other) -> size_t { return 0; }},
         midiMessageId);
   }
};
} // namespace std

namespace base
{
inline constexpr static MidiMessageId midiMessageToId(
   const midi::MidiMessage& msg) noexcept
{
   return mpark::visit(
      midi::overload{
         [](const midi::Message<midi::ControlChange>& msg) -> MidiMessageId {
            return MidiMsgId<midi::ControlChange>{msg.controllerNumber()};
         },
         [](const midi::Message<midi::ControlChangeHighRes>& msg)
            -> MidiMessageId {
            return MidiMsgId<midi::ControlChangeHighRes>{msg.msbCCId,
                                                         msg.lsbCCId};
         },
         [](const midi::Message<midi::NRPN>& msg) -> MidiMessageId {
            return MidiMsgId<midi::NRPN>{msg.idMsb, msg.idLsb};
         },
         [](const midi::Message<midi::RPN>& msg) -> MidiMessageId {
            return MidiMsgId<midi::RPN>{msg.idMsb, msg.idLsb};
         },
         [](const midi::Message<midi::NoteOn>& msg) -> MidiMessageId {
            return MidiMsgId<midi::NoteOn>{msg.noteNumber()};
         },
         [](const midi::Message<midi::NoteOff>& msg) -> MidiMessageId {
            return MidiMsgId<midi::NoteOff>{msg.noteNumber()};
         },
         [](const midi::Message<midi::AfterTouchChannel>& msg)
            -> MidiMessageId { return MidiMsgId<midi::AfterTouchChannel>{}; },
         [](const midi::Message<midi::AfterTouchPoly>& msg) -> MidiMessageId {
            return MidiMsgId<midi::AfterTouchPoly>{msg.noteNumber()};
         },
         [](const midi::Message<midi::PitchBend>& msg) -> MidiMessageId {
            return MidiMsgId<midi::PitchBend>{};
         },
         [](auto&& other) -> MidiMessageId { return mpark::monostate(); }},
      msg);
}

} // namespace base

#endif