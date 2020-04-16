add_subdirectory(docopt.cpp)
add_subdirectory(fmt)
add_subdirectory(googletest)
add_subdirectory(rtmidi)
add_subdirectory(variant)
add_subdirectory(Test EXCLUDE_FROM_ALL)

add_library(Serialize INTERFACE)
target_include_directories(Serialize
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/MetaStuff/include
        ${CMAKE_CURRENT_LIST_DIR}/nlohmann_json
)

set(LOGURU_DIR "${CMAKE_CURRENT_LIST_DIR}/loguru")
add_library(Loguru STATIC "${LOGURU_DIR}/loguru.cpp")
target_include_directories(Loguru PUBLIC ${LOGURU_DIR})
target_link_libraries(Loguru PRIVATE pthread dl)
option(__LOGURU_USE_FMT_LIB__ "use fmtlib" ON)
if(${__LOGURU_USE_FMT_LIB__})
    target_compile_definitions(Loguru PUBLIC "LOGURU_USE_FMTLIB=1")
    #target_link_libraries(Loguru PUBLIC fmt-header-only)
    target_link_libraries(Loguru PUBLIC fmt)
endif(${__LOGURU_USE_FMT_LIB__})


find_package(Qt5 COMPONENTS Core REQUIRED)
set(QTUNIXSIGNALS_DIR "${CMAKE_CURRENT_LIST_DIR}/qt-unix-signals")
add_library(QtUnixSignals STATIC "${QTUNIXSIGNALS_DIR}/sigwatch.cpp")
target_include_directories(QtUnixSignals PUBLIC "${QTUNIXSIGNALS_DIR}")
target_link_libraries(QtUnixSignals PUBLIC Qt5::Core)


set(qt-qml-models-sources
    "qt-qml-models/QQmlObjectListModel.cpp"
    "qt-qml-models/QQmlVariantListModel.cpp"
)
set(qt-qml-models-headers
    "qt-qml-models/QQmlObjectListModel.h"
    "qt-qml-models/QQmlVariantListModel.h"
    "qt-qml-models/QtQmlTricksPlugin_SmartDataModels.h"
)
add_library(qt-qml-models "${qt-qml-models-sources}" "${qt-qml-models-headers}")
target_include_directories(qt-qml-models PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/qt-qml-models")
target_link_libraries(qt-qml-models PUBLIC Qt5::Core)
