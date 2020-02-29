cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(TestApp EXCLUDE_FROM_ALL)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

set(SOURCES
    "InstrumentsBridge.cpp"
    "KitInstrumentBridge.cpp"
)

set(HEADERS
    "InstrumentsBridge.h"
    "KitInstrumentBridge.h"
)

add_library(QtBridges ${SOURCES} ${HEADERS})
target_include_directories(QtBridges PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(QtBridges
    PUBLIC Qt5::Core
    PUBLIC qt-qml-models
)