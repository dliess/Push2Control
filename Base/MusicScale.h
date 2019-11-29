#ifndef MUSIC_SCALE
#define MUSIC_SCALE

#include <array>
#include <cstdint>

namespace music
{

enum Note
{
    C    = 0,
    C_SH = 1,
    D    = 2,
    D_SH = 3,
    E    = 4,
    F    = 5,
    F_SH = 6,
    G    = 7,
    G_SH = 8,
    A    = 9,
    A_SH = 10,
    B    = 11,
    NUM_NOTES_PER_OCTAVE = 12
};


class Scale
{
public:
    constexpr Scale(std::array<uint8_t, 11> distances) noexcept;
    constexpr uint8_t getNoteOffsetOf(uint8_t noteIdx) const noexcept;
    constexpr uint8_t getNumNotes() const noexcept;
    constexpr bool contains(uint8_t noteDiffFromRoot) const noexcept;
private:
    std::array<uint8_t, 12> m_noteOffsets;
    uint16_t                m_mask;
    uint8_t                 m_numNotes;
};

class Scales
{
public:
    enum Type
    {
        Major = 0,
        Minor,
        Dorian,
        Mixolydian,
        Lydian,
        Phrygian,
        Locrian,
        Diminished,
        WholeHalf,
        WholeTone,
        MinorBlues,
        MinorPentatonic,
        MajorPentatonic,
        HarmonicMinor,
        MelodicMinor,
        SuperLocrian,
        Bhairav,
        HungarianMinor,
        MinorGypsy,
        Hirajoshi,
        InSen,
        Iwato,
        Kumoi,
        Pelog,
        NumScales
    };
    constexpr Scales() noexcept;
    constexpr const Scale& operator[](Type type) const noexcept;
private:
    enum NoteDistances
    {
        H  = 1,
        W  = 2,
        H3 = 3,
        H4 = 4
    };
    const std::array<Scale, NumScales> m_scales;
};

} //  namespace music

#include "MusicScale.tcc"

#endif