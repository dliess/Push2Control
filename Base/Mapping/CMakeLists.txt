cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

set(SOURCES
   "NoteMapper.cpp"
   "ParameterMapper.cpp"
   "ValueCurve.cpp"
)

add_library(Mapping STATIC ${SOURCES})

target_include_directories(Mapping PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Mapping
   PUBLIC  MusicDevice
   PUBLIC  Instruments
   PRIVATE Loguru
   PUBLIC  util
)