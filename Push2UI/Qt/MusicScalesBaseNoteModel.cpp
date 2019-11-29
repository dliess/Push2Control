#include "MusicScalesBaseNoteModel.h"
#include "MusicScale.h"

using namespace push2::qt;


MusicScalesBaseNoteModel::MusicScalesBaseNoteModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

QHash<int, QByteArray> MusicScalesBaseNoteModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}

int MusicScalesBaseNoteModel::rowCount(const QModelIndex&) const
{
    return music::Note::NUM_NOTES_PER_OCTAVE;
}

QVariant MusicScalesBaseNoteModel::data(const QModelIndex &index, int role) const
{
    const auto idx = index.row();
    switch(role)
    {
        case NameRole:
        {
            switch(idx)
            {
                case 0: return QString("C");
                case 1: return QString("G");
                case 2: return QString("D");
                case 3: return QString("A");
                case 4: return QString("E");
                case 5: return QString("B");
                case 6: return QString("F");
                case 7: return QString("B") + QChar(0x266D);
                case 8: return QString("E") + QChar(0x266D);
                case 9: return QString("A") + QChar(0x266D);
                case 10: return QString("D") + QChar(0x266D);
                case 11: return QString("G") + QChar(0x266D);
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