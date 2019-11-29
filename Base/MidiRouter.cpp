#include "MidiRouter.h"
#include "MusicDeviceHolder.h"

midi::Router::Router(MusicDeviceHolder& rMusicDeviceHolder) noexcept :
   m_rMusicDeviceHolder(rMusicDeviceHolder)
{
   // TODO: NOT USB HOTPLUG-SAFE
   m_routingData.resize(rMusicDeviceHolder.controllerDevices.size());
   for(auto& inputDevRouteData : m_routingData)
   {
      inputDevRouteData.resize(rMusicDeviceHolder.soundDevices.size());
   }
   for(auto cdIdx = 0; cdIdx < rMusicDeviceHolder.controllerDevices.size(); ++cdIdx)
   {
      auto &controllerDev = rMusicDeviceHolder.controllerDevices.atIdx(cdIdx);
      auto &midiInHandler = controllerDev.second->m_midiInMsgHandler;
      if(!midiInHandler) continue;
      midiInHandler->m_midiIn.registerMidiInCb([this, cdIdx](const midi::MidiMessage& midiMsg){
         for(unsigned sdIdx = 0; sdIdx < m_rMusicDeviceHolder.soundDevices.size(); ++sdIdx)
         {
            const auto& routingData = m_routingData[cdIdx][sdIdx];
            if(routingData.enabled)
            {
               auto& midiOutHandler = m_rMusicDeviceHolder.soundDevices.atIdx(sdIdx).second->m_midiOutHandler;
               if(!midiOutHandler) continue;
               auto& midiOut = midiOutHandler->m_midiOut;
               if(routingData.specialized)
               {
                  mpark::visit(midi::overload{
                     [&routingData, &midiOut](const midi::Message<midi::Clock> & msg){
                        if(routingData.specialized->transmitClockMsg) midiOut.send(msg);
                     },
                     [&routingData, &midiOut](const midi::Message<midi::NoteOn>& msg){
                        const auto map = routingData.specialized->channelMapping[msg.channel() - 1];
                        if(0 == map) return;
                        for(int x = 0; x < sizeof(map) * 8; ++x){
                           if( map & (1 << x) )
                           {
                              midiOut.send(midi::Message<midi::NoteOn>(x+1, msg.noteNumber(), msg.velocity()));
                           }
                        }
                     },
                     [&routingData, &midiOut](const midi::Message<midi::NoteOff>& msg){
                        const auto map = routingData.specialized->channelMapping[msg.channel() - 1];
                        if(0 == map) return;
                        for(int x = 0; x < sizeof(map) * 8; ++x){
                           if( map & (1 << x) )
                           {
                              midiOut.send(midi::Message<midi::NoteOff>(x+1, msg.noteNumber(), msg.velocity()));
                           }
                        }
                     },
                     [&routingData, &midiOut](const midi::Message<midi::AfterTouchPoly>& msg){
                        const auto map = routingData.specialized->channelMapping[msg.channel() - 1];
                        if(0 == map) return;
                        for(int x = 0; x < sizeof(map) * 8; ++x){
                           if( map & (1 << x) )
                           {
                              midiOut.send(midi::Message<midi::AfterTouchPoly>(x+1, msg.noteNumber(), msg.pressure()));
                           }
                        }
                     },
                     [&routingData, &midiOut](const midi::Message<midi::ControlChange>& msg){
                        const auto map = routingData.specialized->channelMapping[msg.channel() - 1];
                        if(0 == map) return;
                        for(int x = 0; x < sizeof(map) * 8; ++x){
                           if( map & (1 << x) )
                           {
                              midiOut.send(midi::Message<midi::ControlChange>(x+1, msg.controllerNumber(), msg.controllerValue()));
                           }
                        }
                     },
                     [&routingData, &midiOut](const midi::Message<midi::ProgramChange>& msg){
                        const auto map = routingData.specialized->channelMapping[msg.channel() - 1];
                        if(0 == map) return;
                        for(int x = 0; x < sizeof(map) * 8; ++x){
                           if( map & (1 << x) )
                           {
                              midiOut.send(midi::Message<midi::ProgramChange>(x+1, msg.programNumber()));
                           }
                        }
                     },
                     [&routingData, &midiOut](const midi::Message<midi::AfterTouchChannel>& msg){
                        const auto map = routingData.specialized->channelMapping[msg.channel() - 1];
                        if(0 == map) return;
                        for(int x = 0; x < sizeof(map) * 8; ++x){
                           if( map & (1 << x) )
                           {
                              midiOut.send(midi::Message<midi::AfterTouchChannel>(x+1, msg.value()));
                           }
                        }
                     },
                     [&routingData, &midiOut](const midi::Message<midi::PitchBend>& msg){
                        const auto map = routingData.specialized->channelMapping[msg.channel() - 1];
                        if(0 == map) return;
                        for(int x = 0; x < sizeof(map) * 8; ++x){
                           if( map & (1 << x) )
                           {
                              midiOut.send(midi::Message<midi::PitchBend>(x+1, msg.value()));
                           }
                        }
                     },
                     [&midiMsg, routingData, &midiOut](const auto& msg){ // TODO: try auto&&
                        if(routingData.specialized->transmitTransportMsg) midiOut.send(msg);
                     }
                  }, midiMsg);
               }
               else
               {
                  midiOut.send(midiMsg);
               }
            }
         }
      });                
   }
}

bool midi::Router::isRoutedTo(int controllerIdx, int soundDeviceIdx) const noexcept
{
   if(!isIdxValid(controllerIdx, soundDeviceIdx)) return false;
   return m_routingData[controllerIdx][soundDeviceIdx].enabled;
}

void midi::Router::toggleRouted(int controllerIdx, int soundDeviceIdx) noexcept
{
   if(!isIdxValid(controllerIdx, soundDeviceIdx)) return;
   m_routingData[controllerIdx][soundDeviceIdx].enabled = !m_routingData[controllerIdx][soundDeviceIdx].enabled;
}

bool midi::Router::hasSpecializedData(int controllerIdx, int soundDeviceIdx) const noexcept
{
   if(!isIdxValid(controllerIdx, soundDeviceIdx)) return false;
   return m_routingData[controllerIdx][soundDeviceIdx].specialized.has_value();
}

void midi::Router::initSpecialized(int controllerIdx, int soundDeviceIdx) noexcept
{
   if(!isIdxValid(controllerIdx, soundDeviceIdx)) return;
   if(m_routingData[controllerIdx][soundDeviceIdx].specialized) return;
   m_routingData[controllerIdx][soundDeviceIdx].specialized.emplace(RoutingDataSpecialized());
   for(auto& cb : m_cb) cb();
}

void midi::Router::clearSpecialized(int controllerIdx, int soundDeviceIdx) noexcept
{
   if(!isIdxValid(controllerIdx, soundDeviceIdx)) return;
   m_routingData[controllerIdx][soundDeviceIdx].specialized.reset();
   for(auto& cb : m_cb) cb();
}

uint16_t midi::Router::getMappingFor(int controllerIdx, int soundDeviceIdx, int sourceChannelIdx) const noexcept
{
   if(!isIdxValid(controllerIdx, soundDeviceIdx)) return 0;
   if(sourceChannelIdx < 0 || sourceChannelIdx > 15) return 0;
   if(!m_routingData[controllerIdx][soundDeviceIdx].specialized) return (1<<sourceChannelIdx);
   return m_routingData[controllerIdx][soundDeviceIdx].specialized->channelMapping[sourceChannelIdx];
}

void midi::Router::setMappingForChannelIdx(int controllerIdx,    int soundDeviceIdx, 
                                           int sourceChannelIdx, int destinationChannelIdx,
                                           bool enable) noexcept
{
   if(!isIdxValid(controllerIdx, soundDeviceIdx)) return;
   if(sourceChannelIdx < 0 || sourceChannelIdx > 15) return;
   if(destinationChannelIdx < 0 || destinationChannelIdx > 15) return;
   if(!m_routingData[controllerIdx][soundDeviceIdx].specialized) return;
   if(enable)
   {
      m_routingData[controllerIdx][soundDeviceIdx].specialized->channelMapping[sourceChannelIdx] |= (1<<destinationChannelIdx);
   }
   else
   {
      m_routingData[controllerIdx][soundDeviceIdx].specialized->channelMapping[sourceChannelIdx] &= ~(1<<destinationChannelIdx);
   }
   for(auto& cb : m_cbSpecSchanged) cb();
}

void midi::Router::registerChangedCb(std::function<void(void)> cb) noexcept
{
   m_cb.push_back(cb);
}

void midi::Router::registerSpecChangedCb(std::function<void(void)> cb) noexcept
{
   m_cbSpecSchanged.push_back(cb);
}

midi::Router::Settings midi::Router::getSettings() const noexcept
{
   midi::Router::Settings settings;
   for(auto cdIdx = 0; cdIdx < m_rMusicDeviceHolder.controllerDevices.size(); ++cdIdx)
   {
      std::unordered_map<std::string, RoutingData> map;
      for(unsigned sdIdx = 0; sdIdx < m_rMusicDeviceHolder.soundDevices.size(); ++sdIdx)
      {
         const auto sdName = m_rMusicDeviceHolder.soundDevices.atIdx(sdIdx).first.toStr();
         map[sdName] = m_routingData[cdIdx][sdIdx];
      }
      const auto cdName = m_rMusicDeviceHolder.controllerDevices.atIdx(cdIdx).first.toStr();
      settings[cdName] = std::move(map);
   }
   return settings;
}

void midi::Router::setSettings(const Settings& settings) noexcept
{
   for(auto& [cdName, map] : settings)
   {
      const auto cdIdx = m_rMusicDeviceHolder.controllerDevices.getIndexOf(cdName);
      if(-1 == cdIdx) continue;
      for(auto& [sdName, routingData] : map)
      {
         const auto sdIdx = m_rMusicDeviceHolder.soundDevices.getIndexOf(sdName);
         if(-1 == sdIdx) continue;
         m_routingData[cdIdx][sdIdx] = routingData;
      }
   }
}

bool midi::Router::isIdxValid(int controllerIdx, int soundDeviceIdx) const noexcept
{
   if(0 > controllerIdx || controllerIdx >= m_routingData.size())
   {
      return false;
   }
   if(0 > soundDeviceIdx || soundDeviceIdx >= m_routingData[controllerIdx].size())
   {
      return false;
   }
   return true;
}

