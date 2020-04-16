cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

set(SOURCES
   "Instruments.cpp"
   "InstrumentsFactory.cpp"
   "KitInstrument.cpp"
   "MelodicInstrument.cpp"
)

add_library(Instruments STATIC ${SOURCES})

target_include_directories(Instruments PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Instruments
   PUBLIC  MusicDevice
   PRIVATE usbmidi
   PRIVATE Loguru
   PUBLIC  util
)