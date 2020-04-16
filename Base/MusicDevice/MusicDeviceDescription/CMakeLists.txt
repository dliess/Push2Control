cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

set(SOURCES
   "MusicDeviceDescription.cpp"
)

add_library(MusicDeviceDescription STATIC ${SOURCES})
target_include_directories(MusicDeviceDescription PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(MusicDeviceDescription
   PUBLIC Util
   PUBLIC midi
)