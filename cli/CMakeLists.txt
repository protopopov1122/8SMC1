cmake_minimum_required(VERSION 2.6)
project(CalX)

include_directories(${CMAKE_SOURCE_DIR})
add_executable(calxcli DevCLI.cpp CLI.cpp main.cpp)
target_link_libraries(calxcli calx dev_emulated)
