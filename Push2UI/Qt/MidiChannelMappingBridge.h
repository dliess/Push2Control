#ifndef PUSH2_QT_MIDI_CHANNEL_MAPPING_BRIDGE_H
#define PUSH2_QT_MIDI_CHANNEL_MAPPING_BRIDGE_H

#include <QObject>
#include <QString>
#include "Push2Device.h"
#include "MidiRouter.h"

namespace push2::qt
{

class MidiChannelMappingBridge : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool channelMappingActive READ channelMappingActive WRITE setChannelMappingActive NOTIFY channelMappingActiveChanged)
public:
    MidiChannelMappingBridge(push2device::Push2Device& rDevice,
                             midi::Router&             rMidiRouter) noexcept;
    Q_INVOKABLE void activate(int inDevIdx, int outDevIdx) noexcept;
    Q_INVOKABLE void deActivate() noexcept;
    Q_INVOKABLE void toggleChannelMappingActive() noexcept;
    Q_INVOKABLE unsigned int getMappingFor(int sourceChannelIdx) const noexcept;
    Q_INVOKABLE void toggleMappingForChannelIdx(int sourceChannelIdx, int destinationChannelIdx) noexcept;
    bool channelMappingActive() const noexcept;
    void setChannelMappingActive(bool on) noexcept;
private:
    push2device::Push2Device& m_rDevice;
    midi::Router&             m_rMidiRouter;
    static constexpr int UNSET = -1;
    int m_inDevIdx{UNSET};
    int m_outDevIdx{UNSET};
signals:
    void channelMappingActiveChanged();
    void channelMappingSpecializationChanged();
};

} // namespace push2::qt

#endif