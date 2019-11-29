#ifndef MUSIC_SCALES_MODEL_H
#define MUSIC_SCALES_MODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QHash>

namespace push2
{
class Pads;
}
namespace push2::qt
{


class MusicScalesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int mode READ mode NOTIFY modeChanged)
public:
    enum ScaleNameRoles {
        NameRole = Qt::UserRole + 1
    };

    MusicScalesModel(Pads &rPads, QObject *parent = nullptr);
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    int currentIndex() const;
    void incCurrentIndex(int increment);

    void toggleMode();
    int mode();

signals:
    void currentIndexChanged();
    void modeChanged();

private:
    Pads &m_rPads;
    void setCurrentIndex(int currentIndex);
    int m_currentIndex{0};
};

} // namespace push2::qt
#endif