#ifndef KIT_INSTRUMENT_BRIDGE_H
#define KIT_INSTRUMENT_BRIDGE_H

#include "KitSoundsModel.h"
#include <memory>

class KitInstrument;

struct KitInstrumentBridge
{
   KitInstrumentBridge(KitInstrument& rKitInstrument) noexcept:
      rKitInstrument(rKitInstrument),
      pKitSoundsModel(std::make_unique<KitSoundsModel>(rKitInstrument.sounds()))
   {}
   KitInstrument& rKitInstrument;
   std::unique_ptr<KitSoundsModel> pKitSoundsModel;
};

#endif