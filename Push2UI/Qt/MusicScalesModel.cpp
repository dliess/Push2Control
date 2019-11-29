#include "MusicScalesModel.h"
#include "MusicScale.h"
#include "Push2Pads.h"

using namespace push2::qt;


MusicScalesModel::MusicScalesModel(Pads &rPads, QObject *parent) :
    m_rPads(rPads),
    QAbstractListModel(parent)
{
}

QHash<int, QByteArray> MusicScalesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}

int MusicScalesModel::rowCount(const QModelIndex&) const
{
    return music::Scales::NumScales;
}

QVariant MusicScalesModel::data(const QModelIndex &index, int role) const
{
    const auto idx = index.row();
    switch(role)
    {
        case NameRole:
        {
            switch(idx)
            {
                case music::Scales::Major: return QString("Major");
                case music::Scales::Minor: return QString("Minor");
                case music::Scales::Dorian: return QString("Dorian");
                case music::Scales::Mixolydian: return QString("Mixolydian");
                case music::Scales::Lydian: return QString("Lydian");
                case music::Scales::Phrygian: return QString("Phrygian");
                case music::Scales::Locrian: return QString("Locrian");
                case music::Scales::Diminished: return QString("Diminished");
                case music::Scales::WholeHalf: return QString("Whole-half");
                case music::Scales::WholeTone: return QString("Whole Tone");
                case music::Scales::MinorBlues: return QString("Minor Blues");
                case music::Scales::MinorPentatonic: return QString("Minor Pentatonic");
                case music::Scales::MajorPentatonic: return QString("Major Pentatonic");
                case music::Scales::HarmonicMinor: return QString("Harmonic Minor");
                case music::Scales::MelodicMinor: return QString("Melodic Minor");
                case music::Scales::SuperLocrian: return QString("Super Locrian");
                case music::Scales::Bhairav: return QString("Bhairav");
                case music::Scales::HungarianMinor: return QString("Hungarian Minor");
                case music::Scales::MinorGypsy: return QString("Minor Gypsy");
                case music::Scales::Hirajoshi: return QString("Hirajoshi");
                case music::Scales::InSen: return QString("In-Sen");
                case music::Scales::Iwato: return QString("Iwato");
                case music::Scales::Kumoi: return QString("Kumoi");
                case music::Scales::Pelog: return QString("Pelog");
                default: break; 
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return QVariant();
}

int MusicScalesModel::currentIndex() const
{
    return m_currentIndex;
}

void MusicScalesModel::incCurrentIndex(int increment)
{
    setCurrentIndex(m_currentIndex + increment);
}

void MusicScalesModel::setCurrentIndex(int currentIndex)
{
    if(currentIndex < 0 || currentIndex >= music::Scales::NumScales)
    {
        return;
    }
    if(m_currentIndex != currentIndex)
    {
        m_currentIndex = currentIndex;
        emit currentIndexChanged();
    }
}

void MusicScalesModel::toggleMode()
{
    m_rPads.toggleMode();
    emit modeChanged();
}

int MusicScalesModel::mode()
{
    return static_cast<int>(m_rPads.getMode());
}

