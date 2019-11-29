#include "MidiChannelMappingBridge.h"

push2::qt::MidiChannelMappingBridge::MidiChannelMappingBridge(push2device::Push2Device& rDevice,
                                                              midi::Router&             rMidiRouter) noexcept :
    m_rDevice(rDevice),
    m_rMidiRouter(rMidiRouter)
{
    m_rMidiRouter.registerChangedCb([this](){
        if(m_inDevIdx != UNSET && m_outDevIdx != UNSET)
        {
            emit channelMappingActiveChanged();
        }
    });
    m_rMidiRouter.registerSpecChangedCb([this](){
        if(m_inDevIdx != UNSET && m_outDevIdx != UNSET)
        {
            emit channelMappingSpecializationChanged();
        }
    });
}

void push2::qt::MidiChannelMappingBridge::activate(int inDevIdx, int outDevIdx) noexcept
{
    m_inDevIdx = inDevIdx;
    m_outDevIdx = outDevIdx;
    emit channelMappingActiveChanged();
}

void push2::qt::MidiChannelMappingBridge::deActivate() noexcept
{
    m_inDevIdx = UNSET;
    m_outDevIdx = UNSET;
}

void push2::qt::MidiChannelMappingBridge::toggleChannelMappingActive() noexcept
{
    setChannelMappingActive(!channelMappingActive());
}

unsigned int push2::qt::MidiChannelMappingBridge::getMappingFor(int sourceChannelIdx) const noexcept
{
    return m_rMidiRouter.getMappingFor(m_inDevIdx, m_outDevIdx, sourceChannelIdx);
}

void push2::qt::MidiChannelMappingBridge::toggleMappingForChannelIdx(int sourceChannelIdx, int destinationChannelIdx) noexcept
{
    const uint16_t mapping = m_rMidiRouter.getMappingFor(m_inDevIdx, m_outDevIdx, sourceChannelIdx);
    const bool enabledNow = mapping & (1 << destinationChannelIdx);
    m_rMidiRouter.setMappingForChannelIdx(m_inDevIdx, m_outDevIdx, sourceChannelIdx, destinationChannelIdx, !enabledNow);
}

bool push2::qt::MidiChannelMappingBridge::channelMappingActive() const noexcept
{
    return m_rMidiRouter.hasSpecializedData(m_inDevIdx, m_outDevIdx);
}

void push2::qt::MidiChannelMappingBridge::setChannelMappingActive(bool on) noexcept
{
    if(on)
    {
        m_rMidiRouter.initSpecialized(m_inDevIdx, m_outDevIdx);
    }
    else
    {
        m_rMidiRouter.clearSpecialized(m_inDevIdx, m_outDevIdx);
    }
}
