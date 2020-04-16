cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(MusicDeviceDescription)

set(SOURCES
   "DeviceDescriptionLoader.cpp"
   "Midi1OutputManager.cpp"
   "MidiInMsgHandler.cpp"
   "MidiOutMsgHandler.cpp"
   "MusicDevice.cpp"
   "MusicDeviceFactory.cpp"
   "MusicDeviceHolder.cpp"
   "SoundPresets.cpp"
)

add_library(MusicDevice STATIC ${SOURCES})

target_include_directories(MusicDevice PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(MusicDevice
   PUBLIC  MusicDeviceDescription
   PUBLIC  midi
   PRIVATE usbmidi
   PRIVATE Loguru
)

option(__INSERT_DUMMY_MIDI_DEVICES__ "insert dummy devices for testing" OFF)
if(${__INSERT_DUMMY_MIDI_DEVICES__})
   target_compile_definitions(MusicDevice PUBLIC "__INSERT_DUMMY_MIDI_DEVICES__")
   target_link_libraries(MusicDevice PRIVATE MidiMediumDummy)
endif(${__INSERT_DUMMY_MIDI_DEVICES__})
