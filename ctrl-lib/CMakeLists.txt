cmake_minimum_required(VERSION 2.6)
project(CalX)


include_directories(${CMAKE_SOURCE_DIR})
add_definitions(-DLOGGING)
file(GLOB_RECURSE SRCS *.cpp)
add_library(calx SHARED ${SRCS})
target_include_directories(calx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
if (WIN32)
	install(TARGETS calx DESTINATION /)
else (WIN32)
	install(TARGETS calx DESTINATION lib)
endif (WIN32)
