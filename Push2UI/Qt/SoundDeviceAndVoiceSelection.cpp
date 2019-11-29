#include "SoundDeviceAndVoiceSelection.h"
#include "MusicDeviceHolder.h"

push2::qt::SoundDeviceAndVoiceSelection::SoundDeviceAndVoiceSelection(MusicDeviceHolder& rMusicDeviceHolder) :
   QObject(),
   m_soundDeviceSelectionModel(rMusicDeviceHolder),
   m_voiceSelectionModel(rMusicDeviceHolder),
   m_savedVoiceIndexes(rMusicDeviceHolder.soundDevices.size())
{}

QAbstractListModel* push2::qt::SoundDeviceAndVoiceSelection::soundDeviceSelectionModel()
{
   return &m_soundDeviceSelectionModel;
}

QAbstractListModel* push2::qt::SoundDeviceAndVoiceSelection::voiceSelectionModel()
{
   return &m_voiceSelectionModel;
}

int push2::qt::SoundDeviceAndVoiceSelection::currentDeviceIndex() const
{
   return m_voiceSelectionModel.currentDeviceIndex();
}

void push2::qt::SoundDeviceAndVoiceSelection::setCurrentDeviceIndex(int index)
{
   m_voiceSelectionModel.setCurrentDeviceIndex(index);
   m_voiceSelectionModel.setCurrentIndex(m_savedVoiceIndexes[index]);
   emit currentDeviceIndexChanged();
   emit currentVoiceIndexChanged();
}

int push2::qt::SoundDeviceAndVoiceSelection::currentVoiceIndex() const
{
   return m_voiceSelectionModel.currentIndex();
}

void push2::qt::SoundDeviceAndVoiceSelection::setCurrentVoiceIndex(int index)
{
   m_savedVoiceIndexes[currentDeviceIndex()] = index;
   m_voiceSelectionModel.setCurrentIndex(index);
   emit currentVoiceIndexChanged();
}
