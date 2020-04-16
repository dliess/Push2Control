cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

set(SOURCES
   "TapTempoHandler.cpp"
   "TempoHandler.cpp"
   "TransportControl.cpp"
)

add_library(TempoAndTransport STATIC ${SOURCES})

target_include_directories(TempoAndTransport PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(TempoAndTransport
   PUBLIC  MusicDevice
   PRIVATE Loguru
   PUBLIC  util
)