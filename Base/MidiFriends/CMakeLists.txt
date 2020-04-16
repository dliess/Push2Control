cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

set(SOURCES
   "MidiDumper.cpp"
   "MidiRouter.cpp"
)

add_library(MidiFriends STATIC ${SOURCES})

target_include_directories(MidiFriends PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(MidiFriends
   PUBLIC  MusicDevice
   PRIVATE Loguru
   PUBLIC  util
   PUBLIC  midi
)