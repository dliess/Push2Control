#ifndef MOTE_MAPPER_H
#define MOTE_MAPPER_H

#include "NoteMapperSettings.h"
#include "Settings.h"
#include "MusicDeviceHolder.h"
#include "Instruments.h"
#include <unordered_map>
#include <mpark/variant.hpp>

class NoteMapper : public utils::Settings<NoteMapper>
{
public:
   NoteMapper(MusicDeviceHolder::MusicDevices& rControllerDevices,
              Instruments&                     rInstruments) noexcept;
   // ============== Settings ===============
   using Settings = std::vector<NoteMapperDescription>;
   Settings getSettings() const noexcept;
   void setSettings(const Settings& settings) noexcept;
   // =======================================
private:
   static constexpr int INVALID_IDX = -1;
   struct KitSoundId
   {
      int kitIndex{INVALID_IDX};
      int soundIndex{INVALID_IDX};
   };
   struct MelodicInstrumentId
   {
      int index{INVALID_IDX};
   };

   using MappingDestination = mpark::variant<KitSoundId, MelodicInstrumentId>;

   struct ControllerDeviceId
   {
      int                deviceIndex{INVALID_IDX};
      std::optional<int> presetIndex;
      bool operator==(const ControllerDeviceId& rhs) const noexcept
      {
         return deviceIndex == rhs.deviceIndex && presetIndex == rhs.presetIndex;
      }
      struct HashFn
      { 
         size_t operator()(const ControllerDeviceId& controllerDeviceId) const noexcept
         { 
            return (controllerDeviceId.deviceIndex << 8) ^
                   (controllerDeviceId.presetIndex ? *controllerDeviceId.presetIndex : 0); 
         } 
      };
   };
   std::unordered_map<ControllerDeviceId, MappingDestination, ControllerDeviceId::HashFn> m_mapping;
   MusicDeviceHolder::MusicDevices& m_rControllerDevices;
   Instruments&                     m_rInstruments;
};


#endif // MOTE_MAPPER_H