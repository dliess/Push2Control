cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

project(Push2Control LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#sudo apt-get install qtbase5-dev
#sudo apt install qtdeclarative5-dev
#sudo apt install qml-module-qtquick
#sudo apt install libasound2-dev
#sudo apt install libgtest-dev
#sudo apt install libusb-1.0-0-dev

find_package(Qt5 COMPONENTS Core Quick REQUIRED)
#find_package(TimeMeasure CONFIG REQUIRED)

add_subdirectory(3rdparty)
include_directories(3rdparty)
add_subdirectory(FrontPanelIO)
add_subdirectory(QuickViewOnFp)
add_subdirectory(Midi)
add_subdirectory(Util)
add_subdirectory(Push2Device)
add_subdirectory(Base)
add_subdirectory(Push2UI)

add_executable(${PROJECT_NAME}
   "main.cpp"
   "resources.qrc"
)

target_link_libraries(${PROJECT_NAME}
   Loguru
   Base
   QuickViewOnFp
   Push2UI
   usbmidi
   QtUnixSignals
)

option(__SIMULATION__MODE__ "use in simulation mode" OFF)
if(${__SIMULATION__MODE__})
   target_compile_definitions(${PROJECT_NAME} PRIVATE "__SIMULATION__MODE__")
   add_subdirectory(FrontPanelSimulation)
endif(${__SIMULATION__MODE__})

option(__INSERT_DUMMY_MIDI_DEVICES__ "insert dummy devices for testing" OFF)
if(${__INSERT_DUMMY_MIDI_DEVICES__})
   target_compile_definitions(${PROJECT_NAME} PRIVATE "__INSERT_DUMMY_MIDI_DEVICES__")
   target_link_libraries(${PROJECT_NAME} MidiMediumDummy)
endif(${__INSERT_DUMMY_MIDI_DEVICES__})

install(TARGETS ${PROJECT_NAME} DESTINATION "")
