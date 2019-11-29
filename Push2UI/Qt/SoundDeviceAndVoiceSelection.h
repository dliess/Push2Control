#ifndef SOUND_DEVICE_AND_VOICE_SELECTION_H
#define SOUND_DEVICE_AND_VOICE_SELECTION_H

#include <QObject>
#include "SoundDeviceSelectionModel.h"
#include "VoiceSelectionModel.h"

class MusicDeviceHolder;

namespace push2::qt
{

class SoundDeviceAndVoiceSelection : public QObject
{
   Q_OBJECT
   Q_PROPERTY(QAbstractListModel* soundDeviceSelectionModel READ soundDeviceSelectionModel NOTIFY soundDeviceSelectionModelChanged)
   Q_PROPERTY(QAbstractListModel* voiceSelectionModel READ voiceSelectionModel NOTIFY voiceSelectionModelChanged)
   Q_PROPERTY(int currentDeviceIndex READ currentDeviceIndex NOTIFY currentDeviceIndexChanged)
   Q_PROPERTY(int currentVoiceIndex READ currentVoiceIndex NOTIFY currentVoiceIndexChanged)
public:
   SoundDeviceAndVoiceSelection(MusicDeviceHolder& rMidiOpenedPorts);
   QAbstractListModel* soundDeviceSelectionModel();
   QAbstractListModel* voiceSelectionModel();
   int currentDeviceIndex() const;
   Q_INVOKABLE void setCurrentDeviceIndex(int index);
   int currentVoiceIndex() const;
   Q_INVOKABLE void setCurrentVoiceIndex(int index);
private:
   SoundDeviceSelectionModel m_soundDeviceSelectionModel;
   VoiceSelectionModel       m_voiceSelectionModel;
   std::vector<int>          m_savedVoiceIndexes;

signals:
   void soundDeviceSelectionModelChanged(); //dummy, make qml property binding happy
   void voiceSelectionModelChanged();       //dummy, make qml property binding happy
   void currentDeviceIndexChanged();
   void currentVoiceIndexChanged();
};

} // namespace push2::qt
#endif