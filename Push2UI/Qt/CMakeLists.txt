cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(Bridges)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

add_library(Push2UIQt STATIC
   "ControllerDeviceAndPresetSelection.cpp"
   "ControllerDeviceParameter.cpp"
   "ControllerDeviceParameterCategoryModel.cpp"
   "ControllerDeviceParameterModel.cpp"
   "ControllerDeviceSelectionModel.cpp"
   "ControllerPresetSelectionModel.cpp"
   "MappingCurveDrawing.cpp"
   "MappingCurveQt.cpp"
   "MidiChannelMappingBridge.cpp"
   "MidiDumperModel.cpp"
   "MidiOpenedInputPortsModel.cpp"
   "MidiOpenedOutputPortsModel.cpp"
   "MusicScalesBaseNoteModel.cpp"
   "MusicScalesModel.cpp"
   "Push2DeviceOverlay.cpp"
   "Push2DeviceQtAdapter.cpp"
   "Push2PadsBridge.cpp"
   "QmlSceneLoader.cpp"
   "SettingsSaverBridge.cpp"
   "SoundDeviceAndVoiceSelection.cpp"
   "SoundDeviceParameter.cpp"
   "SoundDeviceParameterCategoryModel.cpp"
   "SoundDeviceParameterModel.cpp"
   "SoundDeviceSelectionModel.cpp"
   "SoundPresetListModel.cpp"
   "StartableDevicesModel.cpp"
   "TapTempoHandlerBridge.cpp"
   "TempoHandlerBridge.cpp"
   "TransportControlBridge.cpp"
   "ValueCircle.cpp"
   "VoiceSelectionModel.cpp"
)

target_include_directories(Push2UIQt
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Push2UIQt
   PRIVATE Push2UI # TODO: Grrrr circular dependency!!!!
   PUBLIC  Qt5::Core
   PRIVATE Qt5::Gui
   PUBLIC  Qt5::Qml
   PRIVATE Base
   PRIVATE QuickViewOnFp
   PRIVATE Loguru
)