#ifndef MIDI_MESSAGE_IDS_H
#define MIDI_MESSAGE_IDS_H

#include <mpark/variant.hpp>

#include "JsonCast.h"
#include "Meta.h"
#include "Midi.h"

template<midi::MsgType>
struct MidiMsgId
{
};

template<>
struct MidiMsgId<midi::NoteOff>
{
   int note;
};

template<>
struct MidiMsgId<midi::NoteOn>
{
   int note;
};

template<>
struct MidiMsgId<midi::ControlChange>
{
   int id;
};

template<>
struct MidiMsgId<midi::ControlChangeHighRes>
{
   int idMsb;
   int idLsb;
};

template<>
struct MidiMsgId<midi::NRPN>
{
   int idMsb;
   int idLsb;
};

template<>
struct MidiMsgId<midi::RPN>
{
   int idMsb;
   int idLsb;
};

template<>
struct MidiMsgId<midi::AfterTouchPoly>
{
   int note;
};

template<>
struct MidiMsgId<midi::AfterTouchChannel>
{
};

template<>
struct MidiMsgId<midi::PitchBend>
{
};

using MidiMessageId = mpark::variant<
    MidiMsgId<midi::NoteOff>,
    MidiMsgId<midi::NoteOn>,
    MidiMsgId<midi::ControlChange>,
    MidiMsgId<midi::ControlChangeHighRes>,
    MidiMsgId<midi::NRPN>,
    MidiMsgId<midi::RPN>,
    MidiMsgId<midi::AfterTouchPoly>,
    MidiMsgId<midi::AfterTouchChannel>,
    MidiMsgId<midi::PitchBend>
>;

namespace meta
{
template<>
inline auto getClassNameOrIndex<MidiMsgId<midi::NoteOff>>(int i) noexcept
{
   return "MidiNoteOff";
}
template<>
inline auto registerMembers<MidiMsgId<midi::NoteOff>>()
{
   return members(member("note", &MidiMsgId<midi::NoteOff>::note));
}

template<>
inline auto getClassNameOrIndex<MidiMsgId<midi::NoteOn>>(int i) noexcept
{
   return "MidiNoteOn";
}
template<>
inline auto registerMembers<MidiMsgId<midi::NoteOn>>()
{
   return members(member("note", &MidiMsgId<midi::NoteOn>::note));
}

template<>
inline auto getClassNameOrIndex<MidiMsgId<midi::ControlChange>>(int i) noexcept
{
   return "MidiControlChange";
}
template<>
inline auto registerMembers<MidiMsgId<midi::ControlChange>>()
{
   return members(member("id", &MidiMsgId<midi::ControlChange>::id));
}

template<>
inline auto getClassNameOrIndex<MidiMsgId<midi::ControlChangeHighRes>>(int i) noexcept
{
   return "MidiControlChangeHighRes";
}
template<>
inline auto registerMembers<MidiMsgId<midi::ControlChangeHighRes>>()
{
   return members(
       member("idMsb", &MidiMsgId<midi::ControlChangeHighRes>::idMsb),
       member("idLsb", &MidiMsgId<midi::ControlChangeHighRes>::idLsb)
   );
}

template<>
inline auto getClassNameOrIndex<MidiMsgId<midi::NRPN>>(int i) noexcept
{
   return "MidiNRPN";
}
template<>
inline auto registerMembers<MidiMsgId<midi::NRPN>>()
{
   return members(
       member("idMsb", &MidiMsgId<midi::NRPN>::idMsb),
       member("idLsb", &MidiMsgId<midi::NRPN>::idLsb)
    );
}

template<>
inline auto getClassNameOrIndex<MidiMsgId<midi::RPN>>(int i) noexcept
{
   return "MidiRPN";
}
template<>
inline auto registerMembers<MidiMsgId<midi::RPN>>()
{
   return members(
       member("idMsb", &MidiMsgId<midi::RPN>::idMsb),
       member("idLsb", &MidiMsgId<midi::RPN>::idLsb)
    );
}

template<>
inline auto getClassNameOrIndex<MidiMsgId<midi::AfterTouchPoly>>(int i) noexcept
{
   return "MidiAfterTouchPoly";
}
template<>
inline auto registerMembers<MidiMsgId<midi::AfterTouchPoly>>()
{
   return members(member("note", &MidiMsgId<midi::AfterTouchPoly>::note));
}

template<>
inline auto getClassNameOrIndex<MidiMsgId<midi::AfterTouchChannel>>(int i) noexcept
{
   return "MidiAfterTouchChannel";
}
template<>
inline auto registerMembers<MidiMsgId<midi::AfterTouchChannel>>()
{
   return members();
}

template<>
inline auto getClassNameOrIndex<MidiMsgId<midi::PitchBend>>(int i) noexcept
{
   return "MidiPitchBend";
}
template<>
inline auto registerMembers<MidiMsgId<midi::PitchBend>>()
{
   return members();
}

} // namespace meta

#endif