cmake_minimum_required(VERSION 2.6)
project(CalX)


include_directories(${CMAKE_SOURCE_DIR})
add_definitions(-DLOGGING)
file(GLOB_RECURSE SRCS *.cpp)
add_library(calx SHARED ${SRCS})
target_include_directories(calx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
install(TARGETS calx DESTINATION lib)
