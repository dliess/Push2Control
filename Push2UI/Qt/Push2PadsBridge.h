#ifndef PUSH2_PADS_QT_H
#define PUSH2_PADS_QT_H

#include <QObject>
#include "MusicScale.h"

namespace push2{ class Pads; }

namespace push2::qt
{

namespace MusicNote
{
Q_NAMESPACE
enum class Value
{
    C    = music::Note::C,
    C_SH = music::Note::C_SH,
    D    = music::Note::D,
    D_SH = music::Note::D_SH,
    E    = music::Note::E,
    F    = music::Note::F,
    F_SH = music::Note::F_SH,
    G    = music::Note::G,
    G_SH = music::Note::G_SH,
    A    = music::Note::A,
    A_SH = music::Note::A_SH,
    B    = music::Note::B,
    NUM_NOTES_PER_OCTAVE = music::Note::NUM_NOTES_PER_OCTAVE
};
Q_ENUM_NS(Value)
} // namespace MusicNote

namespace MusicScales
{
Q_NAMESPACE
enum class Value
{
    Major = music::Scales::Type::Major,
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
Q_ENUM_NS(Value)
} // namespace MusicScales

class PadsBridge : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int rootNote READ rootNote NOTIFY rootNoteChanged)
    Q_PROPERTY(int musicScale READ musicScale NOTIFY musicScaleChanged)
    Q_PROPERTY(int mode READ mode NOTIFY modeChanged)
    Q_PROPERTY(int octave READ octave NOTIFY octaveChanged)
public:
    explicit PadsBridge(push2::Pads& rPads, QObject* parent = Q_NULLPTR);
   
    Q_INVOKABLE void activate();
    Q_INVOKABLE void deActivate();
    
    int rootNote() const;   
    Q_INVOKABLE void setRootNote(MusicNote::Value newRootNote);
    int musicScale() const;
    Q_INVOKABLE void setMusicScale(MusicScales::Value musicScale);
    Q_INVOKABLE void toggleMode();
    int mode() const;
    int octave() const;
    Q_INVOKABLE void incOctave();
    Q_INVOKABLE void decOctave();

signals:
    void rootNoteChanged();
    void musicScaleChanged();
    void modeChanged();
    void octaveChanged();

private:
    push2::Pads& m_rPads;
};

} // namespace push2::qt
#endif