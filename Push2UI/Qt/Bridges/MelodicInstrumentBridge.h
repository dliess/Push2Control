#ifndef MELODIC_INSTRUMENT_BRIDGE_H
#define MELODIC_INSTRUMENT_BRIDGE_H

#include "MelodicInstrumentVoicesModel.h"
#include <memory>

class  MelodicInstrument;

struct MelodicInstrumentBridge
{
   MelodicInstrumentBridge(MelodicInstrument& rMelodicInstrument) noexcept:
      rMelodicInstrument(rMelodicInstrument),
      pMelodicInstrumentVoicesModel(std::make_unique<MelodicInstrumentVoicesModel>(rMelodicInstrument.voices()))
   {}
   MelodicInstrument& rMelodicInstrument;
   std::unique_ptr<MelodicInstrumentVoicesModel> pMelodicInstrumentVoicesModel;
};

#endif