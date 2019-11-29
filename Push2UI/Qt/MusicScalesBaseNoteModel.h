#ifndef MUSIC_SCALES_BASE_NOTE_MODEL_H
#define MUSIC_SCALES_BASE_NOTE_MODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QHash>

namespace push2::qt
{

class MusicScalesBaseNoteModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum BaseNoteRole {
        NameRole = Qt::UserRole + 1
    };

    MusicScalesBaseNoteModel(QObject *parent = nullptr);
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

} // namespace push2::qt
#endif