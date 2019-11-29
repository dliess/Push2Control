#include "MusicScale.h"

constexpr music::Scale::Scale(std::array<uint8_t, 11> distances) noexcept :
    m_noteOffsets({0}),
    m_mask(1),
    m_numNotes(1)
{
    uint8_t noteOffset = 0;
    for(auto i = 0; (i < distances.size()) && distances[i]; ++i)
    {
        noteOffset += distances[i];
        m_noteOffsets[i+1] = noteOffset;
        m_mask |= (1 << noteOffset);
        m_numNotes++; 
    }
}

constexpr uint8_t music::Scale::getNoteOffsetOf(uint8_t noteIdx) const noexcept
{
    return m_noteOffsets[noteIdx % m_numNotes] + ((noteIdx / m_numNotes) * NUM_NOTES_PER_OCTAVE);
}

constexpr uint8_t music::Scale::getNumNotes() const noexcept
{
    return m_numNotes;
}

constexpr bool music::Scale::contains(uint8_t noteDiffFromRoot) const noexcept
{
    return m_mask & (1 << (noteDiffFromRoot % NUM_NOTES_PER_OCTAVE));
}


constexpr music::Scales::Scales() noexcept :
    m_scales({ Scale({W, W, H, W, W, W}),  // Major
               Scale({W, H, W, W, H, W}),  // Minor
               Scale({W, H, W, W, W, H}),  // Dorian
               Scale({W, W, H, W, W, H}),  // Mixolydian
               Scale({W, W, W, H, W, W}),  // Lydian
               Scale({H, W, W, W, H, W}),  // Phrygian
               Scale({W, W, H, H, W, W}),  // Locrian
               Scale({H, W, H, W, H, W, H}),  // Diminished
               Scale({W, H, W, H, W, H, W}),  // WholeHalf
               Scale({W, W, W, W, W}),     // WholeTone
               Scale({H3, W, H, H, H3}),   // MinorBlues
               Scale({H3, W, W, H3}),      // MinorPentatonic
               Scale({W, W, H3, W}),       // MajorPentatonic
               Scale({W, H, W, W, W, H}),  // HarmonicMinor
               Scale({W, H, W, W, W, W}),  // MelodicMinor
               Scale({H, W, H, W, W, W}),  // SuperLocrian
               Scale({H, H3, H, W, W, H}), // Bhairav
               Scale({W, H, H3, H, H, H3}),// HungarianMinor
               Scale({W, H, H3, H, H, W}), // MinorGypsy
               Scale({H, H4, H, H4}),      // Hirajoshi
               Scale({H, H4, W, H3}),      // InSen
               Scale({H, H4, H, H4}),      // Iwato
               Scale({H, H4, W, H}),       // Kumoi
               Scale({H, W, H3, H, H, W})  // Pelog
    })
{}

constexpr const music::Scale& music::Scales::operator[](Type type) const noexcept
{
    return m_scales[type];
}
