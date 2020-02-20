#ifndef CONTROLLER_DEVICE_AND_PRESET_SELECTION_H
#define CONTROLLER_DEVICE_AND_PRESET_SELECTION_H

#include <QObject>
#include "ControllerDeviceSelectionModel.h"
#include "ControllerPresetSelectionModel.h"

struct MusicDeviceHolder;

namespace push2::qt
{

class ControllerDeviceAndPresetSelection : public QObject
{
   Q_OBJECT
   Q_PROPERTY(QAbstractListModel* controllerDeviceSelectionModel READ controllerDeviceSelectionModel NOTIFY controllerDeviceSelectionModelChanged)
   Q_PROPERTY(QAbstractListModel* controllerPresetSelectionModel READ controllerPresetSelectionModel NOTIFY controllerPresetSelectionModelChanged)
   Q_PROPERTY(int currentDeviceIndex READ currentDeviceIndex NOTIFY currentDeviceIndexChanged)
   Q_PROPERTY(int currentPresetIndex READ currentPresetIndex NOTIFY currentPresetIndexChanged)
public:
   ControllerDeviceAndPresetSelection(MusicDeviceHolder& rMidiOpenedPorts);
   QAbstractListModel* controllerDeviceSelectionModel();
   QAbstractListModel* controllerPresetSelectionModel();
   int currentDeviceIndex() const;
   Q_INVOKABLE void setCurrentDeviceIndex(int index);
   int currentPresetIndex() const;
   Q_INVOKABLE void setCurrentPresetIndex(int index);
private:
   ControllerDeviceSelectionModel m_controllerDeviceSelectionModel;
   ControllerPresetSelectionModel m_controllerPresetSelectionModel;
   std::vector<int>               m_savedPresetIndexes;

signals:
   void controllerDeviceSelectionModelChanged(); // dummy, make qml property binding happy
   void controllerPresetSelectionModelChanged(); // dummy, make qml property binding happy
   void currentDeviceIndexChanged();
   void currentPresetIndexChanged();
};

} // namespace push2::qt
#endif