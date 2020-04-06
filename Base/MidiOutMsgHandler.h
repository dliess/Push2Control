#ifndef MIDI_OUT_MSG_HANDLER_H
#define MIDI_OUT_MSG_HANDLER_H

#include "MidiMessage.h"
#include "MusicDeviceDescription.h"
#include "Midi1OutputManager.h"
#include <optional>
#include <memory>

namespace midi { class Router; }

class MidiOutMsgHandler
{
public:
    MidiOutMsgHandler(std::unique_ptr<midi::IMidiOutMedium>   pMedium,
                      std::shared_ptr<MusicDeviceDescription> pDescr) noexcept;
    void sendSoundParameter(uint32_t voiceId, uint32_t parameterId, float value) noexcept;
    void quarterNotePer24Tick() noexcept;
    void startSequencer() noexcept;
    void stopSequencer() noexcept;
    bool sendParameterDumpRequest() noexcept;
    void processMidiOutBuffers() noexcept;

    void noteOn(int voiceIndex, int note, float velocity) noexcept;
    void noteOff(int voiceIndex, int note, float velocity) noexcept;
    void pitchBend(int voiceIndex, float value) noexcept;

    friend midi::Router;
private:
    midi::Midi1OutputManager                          m_midiOut;
    std::shared_ptr<MusicDeviceDescription>           m_pDescr;
    float m_pitchBendFactor{1.0};
};

#endif 