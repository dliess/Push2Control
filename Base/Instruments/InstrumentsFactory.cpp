#include "InstrumentsFactory.h"

#include "Instruments.h"
#include "MusicDeviceHolder.h"

base::InstrumentsFactory::InstrumentsFactory(
   Instruments& rInstruments, MusicDeviceHolder& rMusicDeviceHolder) noexcept :
   m_rInstruments(rInstruments)
{
   rMusicDeviceHolder.soundDevices.registerForAdd(
      [this, &rMusicDeviceHolder](const MusicDeviceId& id) {
         add(rMusicDeviceHolder.soundDevices.at(id));
      });
   rMusicDeviceHolder.soundDevices.registerForRemove(
      [this, &rMusicDeviceHolder](const MusicDeviceId& id) {
         remove(rMusicDeviceHolder.soundDevices.at(id));
      });
}

void base::InstrumentsFactory::add(std::shared_ptr<MusicDevice> pMusicDevice)
{
   assert(pMusicDevice->pDescr->soundSection);
   switch (pMusicDevice->pDescr->soundSection->defaultInstrumentType)
   {
      case SoundSection::DefaultInstrumentType::DrumKit:
      {
         KitInstrument kitInstrument;
         kitInstrument.setName(pMusicDevice->pDescr->productName);
         for (int i = 0; i < pMusicDevice->pDescr->soundSection->voices.size();
              ++i)
         {
            KitSound kitSound;
            kitSound.name = pMusicDevice->pDescr->soundSection->voices[i].name;
            kitSound.voices[0].soundDeviceId = pMusicDevice->deviceId();
            kitSound.voices[0].pSoundDevice  = pMusicDevice;
            kitSound.voices[0].voiceIndex    = i;
            kitSound.voices[0].noteOffset    = 0;
            kitInstrument.add(kitSound);
         }
         m_rInstruments.kitInstruments.push_back(kitInstrument);
         break;
      }
      case SoundSection::DefaultInstrumentType::InstrumentPerVoice:
      {
         for (int i = 0; i < pMusicDevice->pDescr->soundSection->voices.size();
              ++i)
         {
            MelodicInstrument melodicInstrument;
            std::string name = pMusicDevice->pDescr->productName;
            if (pMusicDevice->pDescr->soundSection->voices.size() > 1)
               name = name + " - " + std::to_string(i + 1);
            melodicInstrument.setName(name);
            melodicInstrument.addVoice(MelodicInstrumentVoice{
               pMusicDevice->deviceId(), pMusicDevice, i});
            m_rInstruments.melodicInstruments.push_back(melodicInstrument);
         }
         break;
      }
      case SoundSection::DefaultInstrumentType::OnePolyphonicInstrument:
      {
         MelodicInstrument melodicInstrument;
         melodicInstrument.setName(pMusicDevice->pDescr->productName);
         for (int i = 0; i < pMusicDevice->pDescr->soundSection->voices.size();
              ++i)
         {
            melodicInstrument.addVoice(MelodicInstrumentVoice{
               pMusicDevice->deviceId(), pMusicDevice, i});
         }
         m_rInstruments.melodicInstruments.push_back(melodicInstrument);
         break;
      }
      default:
      {
         LOG_F(ERROR, "INTERNAL ERROR");
         break;
      }
   }
}

void base::InstrumentsFactory::remove(std::shared_ptr<MusicDevice> pMusicDevice)
{
   // TODO!!!
}
