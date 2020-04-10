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
#include "DoubleBufferedMessageDrain.h"
#include "MusicDeviceDescription.h"

namespace midi
{
class Router;
}
namespace midi
{
class Dumper;
}

namespace base
{

class MidiInMsgHandler
{
public:
   MidiInMsgHandler(std::unique_ptr<midi::IMidiInMedium> pMedium,
                    std::shared_ptr<MusicDeviceDescription> pDescr);

   using Cb =
      std::function<void(uint32_t voiceId, uint32_t parameterId, float value)>;
   void registerForSoundParameter(Cb cb) noexcept;
   void registerForControlParameter(Cb cb) noexcept;
   void processMidiInBuffers();

   friend midi::Router;
   friend midi::Dumper;

private:
   midi::Midi1Input<midi::DoubleBufferedMessageDrain> m_midiIn;
   std::shared_ptr<MusicDeviceDescription> m_pDescr;

   // -------------------------
   // Main Map
   // -------------------------

   // Map keys
   struct MapKey
   {
      MapKey(const midi::MidiMessage& midiMsg) noexcept;
      midi::MsgType midiMsgType{midi::NoValidMsg};
      uint16_t id{0};

      constexpr bool operator==(const MapKey& rhs) const noexcept
      {
         return id == rhs.id && midiMsgType == rhs.midiMsgType;
      }
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

   using MapDest = mpark::variant<SoundDevParameterId, ctrldev::EventId>;

   std::unordered_map<MapKey, MapDest, MapKey::HashFn> m_map;

   // -------------------------
   // MPE Map
   // -------------------------
   mutable std::array<std::optional<ctrldev::WidgetCoord>, midi::NUM_CHANNELS> m_mpeMap;


   // -------------------------
   // Callback containers
   // -------------------------
   std::vector<Cb> m_soundParameterCbs;
   std::vector<Cb> m_controlParameterCbs; // TODO : useless

   void initMappingCaches() noexcept;
   void handleSoundDevParameterRouting(const SoundDevParameterId& id, 
                                       const midi::MidiMessage& midiMsg) const noexcept;
   void handleControllerParameterRouting(ctrldev::EventId id, 
                                         const midi::MidiMessage& midiMsg) const noexcept;
};

} // namespace base

#endif