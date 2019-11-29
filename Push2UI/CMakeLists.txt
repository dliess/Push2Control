cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(Qt)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

add_library(Push2UI STATIC
   "Push2Pads.cpp"
)

target_include_directories(Push2UI
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Push2UI
   PUBLIC  Push2UIQt
   PRIVATE Base
   PUBLIC  Push2DeviceTopology
   PRIVATE FpBase
)

if(${__SIMULATION__MODE__})
   target_include_directories(Push2UI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Push2DeviceSim)
   target_link_libraries(Push2UI PUBLIC FpSimulation)
else()
   target_link_libraries(Push2UI PUBLIC Push2Device)
endif(${__SIMULATION__MODE__})
