cmake_minimum_required(VERSION 3.1)
project(CalX)

if (EXISTS ${THIRD_PARTY}/catch.hpp)
  file(GLOB_RECURSE SOURCES *.cpp)
  file(GLOB_RECURSE DEV_EMULATED ${CMAKE_SOURCE_DIR}/device/emulated/*.cpp)
  add_executable(calx_tests ${SOURCES} ${DEV_EMULATED})
  target_include_directories(calx_tests PUBLIC ${CMAKE_SOURCE_DIR}/headers ${THIRD_PARTY})
  target_link_libraries(calx_tests calx)
  add_custom_command(TARGET calx_tests POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:calx_tests> ${build_directory}
  )
  add_dependencies(calx_tests calx-init-target)
endif (EXISTS ${THIRD_PARTY}/catch.hpp)
