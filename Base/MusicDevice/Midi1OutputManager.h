#ifndef MIDI_1_OUTPUT_THREAD_SAFE_H
#define MIDI_1_OUTPUT_THREAD_SAFE_H

#include "Midi1Output.h"
#include "MidiMessage.h"
#include "Midi.h"
#include "DoubleBuffered.h"
#include "UniqueCCMsgVector.h"

#include <vector>
#include <cstdint>

namespace midi
{

class Midi1OutputManager
{
public:
    Midi1OutputManager(std::unique_ptr<IMidiOutMedium> pMedium);
    Midi1OutputManager(const Midi1OutputManager&) = delete;
    Midi1OutputManager& operator=(const Midi1OutputManager&) = delete;
    void noteOn(uint8_t channel, uint8_t note, uint8_t velocity) noexcept;
    void noteOff(uint8_t channel, uint8_t note, uint8_t velocity) noexcept;
    void pitchBend(uint8_t channel, int16_t value) noexcept;
    void controlParameter(uint8_t channel, uint8_t ccId, uint8_t value) noexcept;
    void sysEx(const std::vector<uint8_t>& msg) noexcept;
    void send(const MidiMessage& msg) noexcept;
    void processBuffers() noexcept;
private:
    Midi1Output m_midi1Output;
    util::DoubleBuffered<UniqueCCMsgVector>                 m_ccBuf;
    util::DoubleBuffered<std::vector<MidiMessage>>          m_msgBuf;
    util::DoubleBuffered<std::vector<std::vector<uint8_t>>> m_sysExBuf;
};

} // namespace midi

#endif