cmake_minimum_required(VERSION 3.8)

set (CMAKE_CXX_STANDARD 17)

set(SOURCES
    "StringCast.cpp"
)

add_library(UtilSerialize STATIC ${SOURCES})
target_include_directories(UtilSerialize
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(UtilSerialize PUBLIC Serialize)