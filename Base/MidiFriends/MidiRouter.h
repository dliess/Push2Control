#ifndef MIDI_ROUTER_H
#define MIDI_ROUTER_H

#include <cstdint>
#include <vector>
#include <array>
#include <optional>
#include <functional>
#include "MidiMessage.h"
#include "Settings.h"
#include "Meta.h"
#include "JsonCast.h"

struct MusicDeviceHolder;

static constexpr int NUM_MIDI_CHANNELS = 16;

struct RoutingDataSpecialized
{
   bool transmitClockMsg{true};
   bool transmitTransportMsg{true};
   std::array<uint16_t, NUM_MIDI_CHANNELS> channelMapping;
};

struct RoutingData
{
   bool enabled{false};
   std::optional<RoutingDataSpecialized> specialized;
};

namespace meta
{

template <>
inline auto registerMembers<RoutingDataSpecialized>()
{
   return members(
      member("transmitClockMsg", &RoutingDataSpecialized::transmitClockMsg),
      member("transmitTransportMsg", &RoutingDataSpecialized::transmitTransportMsg),
      member("channelMapping", &RoutingDataSpecialized::channelMapping)
   );
}

template <>
inline auto registerMembers<RoutingData>()
{
   return members(
      member("enabled", &RoutingData::enabled),
      member("specialized", &RoutingData::specialized)
   );
}

} // namespace meta

namespace midi
{

class Router : public utils::Settings<Router>
{
public:
   Router(MusicDeviceHolder& rMusicDeviceHolder) noexcept;
   bool isRoutedTo(int controllerIdx, int soundDeviceIdx) const noexcept;
   void toggleRouted(int controllerIdx, int soundDeviceIdx) noexcept;
   bool hasSpecializedData(int controllerIdx, int soundDeviceIdx) const noexcept;
   void initSpecialized(int controllerIdx, int soundDeviceIdx) noexcept;
   void clearSpecialized(int controllerIdx, int soundDeviceIdx) noexcept;
   uint16_t getMappingFor(int controllerIdx, int soundDeviceIdx, int sourceChannelIdx) const noexcept;
   void setMappingForChannelIdx(int controllerIdx, int soundDeviceIdx, int sourceChannelIdx, int destinationChannelIdx, bool enable) noexcept;
   void registerChangedCb(std::function<void(void)> cb) noexcept;
   void registerSpecChangedCb(std::function<void(void)> cb) noexcept;
   // ============== Settings ===============
   using Settings = std::unordered_map<std::string, std::unordered_map<std::string, RoutingData>>;
   Settings getSettings() const noexcept;
   void setSettings(const Settings& settings) noexcept;
   // =======================================
private:
   MusicDeviceHolder&                    m_rMusicDeviceHolder;
   std::vector<std::vector<RoutingData>> m_routingData;
   std::vector<std::function<void(void)>>  m_cb;
   std::vector<std::function<void(void)>>  m_cbSpecSchanged;
   bool isIdxValid(int controllerIdx, int soundDeviceIdx) const noexcept;
};

} // namespace midi

#endif