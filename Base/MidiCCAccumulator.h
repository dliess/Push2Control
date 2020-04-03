#ifndef MIDI_CC_ACCUMULATOR_H
#define MIDI_CC_ACCUMULATOR_H

#include <array>
#include <functional>
#include <optional>

#include "MidiMessage.h"

namespace midi
{

template <typename Consumer>
class CCAccumulator
{
public:
   CCAccumulator(Consumer& rConsumer) noexcept;
   bool setPair(int msbId, int lsbId) noexcept;
   void operator()(const midi::Message<midi::ControlChange>& ccMsg) const
      noexcept;

private:
   struct Data
   {
      enum class Role
      {
         SingleValue,
         MSB,
         LSB
      };
      static constexpr int NO_PAIR = -1;
      Role role{Role::SingleValue};
      int pairId{NO_PAIR};
   };

   std::array<Data, 128> m_data;
   std::optional<midi::ControlChange> m_lastReceivedCcMsb;

   Consumer& m_rConsumer;
};

} // namespace midi

#include "MidiCCAccumulator.tcc"

#endif