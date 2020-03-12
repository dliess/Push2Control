#ifndef KIT_SOUND_BRIDGE_H
#define KIT_SOUND_BRIDGE_H

#include "KitSoundVoicesModel.h"
#include <memory>

class KitSound;

struct KitSoundBridge
{
   KitSoundBridge(KitSound& rKitSound) noexcept:
      rKitSound(rKitSound),
      pKitSoundsModel(std::make_unique<KitSoundVoicesModel>(rKitSound.voices))
   {}
   KitSound& rKitSound;
   std::unique_ptr<KitSoundVoicesModel> pKitSoundsModel;
};

#endif