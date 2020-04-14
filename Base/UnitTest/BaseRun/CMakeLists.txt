cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

add_executable(BaseRun "main.cpp")
target_link_libraries(BaseRun
   Base
)