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
   using MapType = std::unordered_map<ControllerDeviceId, MappingDestination, ControllerDeviceId::HashFn>;
   MapType                          m_mapping;
   MusicDeviceHolder::MusicDevices& m_rControllerDevices;
   Instruments&                     m_rInstruments;

   template <typename T>
   static std::optional<int> findIndexOfElementByName(const T& container, const std::string& name) noexcept
   {
      for(int i = 0; i < container.size(); ++i)
      {
         if(container[i].name() == name) return i;
      }
      return std::nullopt;
   }

   void onNoteChange(int presetId, bool on, int note, float velocity) noexcept;
   void onPitchBendChange(int presetId, int note, float pitchBend) noexcept;
};


#endif // MOTE_MAPPER_H