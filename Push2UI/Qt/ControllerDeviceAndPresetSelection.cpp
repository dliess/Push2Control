#include "ControllerDeviceAndPresetSelection.h"
#include "MusicDeviceHolder.h"

push2::qt::ControllerDeviceAndPresetSelection::ControllerDeviceAndPresetSelection(MusicDeviceHolder& rMusicDeviceHolder) :
   QObject(),
   m_controllerDeviceSelectionModel(rMusicDeviceHolder),
   m_controllerPresetSelectionModel(rMusicDeviceHolder),
   m_savedPresetIndexes(rMusicDeviceHolder.controllerDevices.size())
{}

QAbstractListModel* push2::qt::ControllerDeviceAndPresetSelection::controllerDeviceSelectionModel()
{
   return &m_controllerDeviceSelectionModel;
}

QAbstractListModel* push2::qt::ControllerDeviceAndPresetSelection::controllerPresetSelectionModel()
{
   return &m_controllerPresetSelectionModel;
}

int push2::qt::ControllerDeviceAndPresetSelection::currentDeviceIndex() const
{
   return m_controllerPresetSelectionModel.currentDeviceIndex();
}

void push2::qt::ControllerDeviceAndPresetSelection::setCurrentDeviceIndex(int index)
{
   m_controllerPresetSelectionModel.setCurrentDeviceIndex(index);
   m_controllerPresetSelectionModel.setCurrentIndex(m_savedPresetIndexes[index]);
   emit currentDeviceIndexChanged();
   emit currentPresetIndexChanged();
}

int push2::qt::ControllerDeviceAndPresetSelection::currentPresetIndex() const
{
   return m_controllerPresetSelectionModel.currentIndex();
}

void push2::qt::ControllerDeviceAndPresetSelection::setCurrentPresetIndex(int index)
{
   m_savedPresetIndexes[currentDeviceIndex()] = index;
   m_controllerPresetSelectionModel.setCurrentIndex(index);
   emit currentPresetIndexChanged();
}
