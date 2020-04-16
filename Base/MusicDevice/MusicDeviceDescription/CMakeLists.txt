cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

add_library(MusicDeviceDescription INTERFACE)
target_include_directories(MusicDeviceDescription INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(MusicDeviceDescription
   INTERFACE Util
   INTERFACE midi
)