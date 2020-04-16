cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(Instruments)
add_subdirectory(Mapping)
add_subdirectory(MidiFriends)
add_subdirectory(MusicDevice)
add_subdirectory(TempoAndTransport)
add_subdirectory(UnitTest)

set(SOURCES
   "Base.cpp"
)

add_library(Base STATIC ${SOURCES})

target_include_directories(Base
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Base
   PUBLIC  Instruments
   PUBLIC  Mapping
   PUBLIC  MidiFriends
   PUBLIC  MusicDevice
   PUBLIC  TempoAndTransport
   PUBLIC  Util
   PRIVATE Loguru
   PRIVATE usbmidi
)