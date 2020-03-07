cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 5.12.7 EXACT
    COMPONENTS Core Quick REQUIRED)

set(SOURCES
    "CarsModel.cpp"
    "main.cpp"
    "resources.qrc"
    "ParameterModel.cpp"
)

set(HEADERS
    "BusinessLogic.h"
    "CarsModel.h"
    "ParameterModel.h"
)

add_executable(QtBridgesTestApp ${SOURCES} ${HEADERS})
target_link_libraries(QtBridgesTestApp
    Qt5::Core
    Qt5::Quick
)
