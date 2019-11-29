cmake_minimum_required(VERSION 3.8)
 
set (CMAKE_CXX_STANDARD 17)

set(SOURCES "UT_main.cpp"
            "UT_KeyValueVector.cpp"
            "UT_JsonParseOptionals.cpp"
            "UT_JsonParseVariants.cpp"
            "UT_Overload.cpp"
            "UT_Ringbuffer.cpp"
            "UT_ThreadedLoop.cpp"
)

add_executable(UtilsTest ${SOURCES})
target_link_libraries(UtilsTest gtest pthread Util mpark_variant midi)

