cmake_minimum_required(VERSION 3.1)
project(CalX)

if (EXISTS ${THIRD_PARTY}/catch.hpp)
	include_directories(${CMAKE_SOURCE_DIR}/headers ${THIRD_PARTY})
  file(GLOB_RECURSE SOURCES *.cpp)
  add_executable(calx_tests ${SOURCES})
  target_link_libraries(calx_tests calx dev_emulated)
  add_custom_command(TARGET calx_tests POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:calx_tests> ${build_directory}
  )
  add_dependencies(calx_tests calx-init-target)
endif (EXISTS ${THIRD_PARTY}/catch.hpp)
