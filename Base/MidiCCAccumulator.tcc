#include "MidiCCAccumulator.h"

template <typename Consumer>
midi::CCAccumulator<Consumer>::CCAccumulator(Consumer& rConsumer) noexcept :
   m_rConsumer(rConsumer)
{
}

template <typename Consumer>
bool midi::CCAccumulator<Consumer>::setPair(int msbId, int lsbId) noexcept
{
   if (msbId < 0 || msbId >= m_data.size())
      return false;
   if (lsbId < 0 || lsbId >= m_data.size())
      return false;

   m_data[msbId].role   = Data::Role::MSB;
   m_data[msbId].pairId = lsbId;
   m_data[msbId].role   = Data::Role::LSB;
   m_data[lsbId].pairId = msbId;
   return true;
}

template <typename Consumer>
void midi::CCAccumulator<Consumer>::operator()(
   const midi::Message<midi::ControlChange>& ccMsg) const noexcept
{
   switch (m_data[ccMsg.controllerNumber()].role)
   {
      case Data::Role::SingleValue:
      {
         if (m_lastReceivedCcMsb)
         {
            m_lastReceivedCcMsb.reset();
         }
         m_rConsumer(ccMsg);
         break;
      }
      case Data::Role::MSB:
      {
         m_lastReceivedCcMsb.emplace(ccMsg);
         break;
      }
      case Data::Role::LSB:
      {
         if (m_lastReceivedCcMsb)
         {
            m_rConsumer(*m_lastReceivedCcMsb, ccMsg);
         }
         break;
      }
   }
}
