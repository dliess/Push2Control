#ifndef MIDI_OPENED_OUTPUT_PORTS_MODEL_H
#define MIDI_OPENED_OUTPUT_PORTS_MODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QHash>
#include "MusicDeviceHolder.h"

namespace midi{ class Router; }

namespace push2::qt
{

class MidiOpenedOutputPortsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex)
public:
    enum MidiPortRoles {
        NameRole = Qt::UserRole + 1,
        PicturePath,
        SelectedRole,
        HasSpecializedChannelMapping
    };

    MidiOpenedOutputPortsModel(MusicDeviceHolder& rMidiOpenedPorts,
                               midi::Router &rMidiRouter,
                               QObject *parent = nullptr);
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    int currentIndex() const;
    Q_INVOKABLE void setCurrentIndex(int currentIndex);
    Q_INVOKABLE void toggleRouted(int idx);
    Q_INVOKABLE QString getDevName(int idx) const noexcept;
    Q_INVOKABLE QUrl getDevPicturePath(int idx) const noexcept;

private:
    MusicDeviceHolder& m_rMidiOpenedPorts;
    midi::Router&      m_rMidiRouter;
    int                m_currentIndex{0};
    void update();
};

} // namespace push2::qt
#endif