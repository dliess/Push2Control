cmake_minimum_required(VERSION 3.8)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(Serialize)
add_subdirectory(UnitTest EXCLUDE_FROM_ALL)

set(SOURCES
    "FileReader.cpp"
    "HomeDir.cpp"
)

add_library(Util STATIC ${SOURCES})
target_include_directories(Util
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(Util PUBLIC UtilSerialize PUBLIC Loguru PUBLIC -lstdc++fs)