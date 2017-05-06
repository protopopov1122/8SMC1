cmake_minimum_required(VERSION 3.1)
project(CalX)

set (CMAKE_CXX_STANDARD 11)
include_directories(${CMAKE_SOURCE_DIR})
add_library(dev_emulated SHARED EmuMotor.cpp EmuInstrument.cpp EmuDeviceManager.cpp)
target_link_libraries(dev_emulated calx)
add_custom_command(TARGET dev_emulated POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:dev_emulated> ${build_directory}
)
if (WIN32)
	install(TARGETS dev_emulated DESTINATION / COMPONENT devapi_emulated)
else (WIN32)
	install(TARGETS dev_emulated DESTINATION lib/calx_devapi COMPONENT devapi_emulated)
endif (WIN32)
set(CPACK_COMPONENT_DEVAPI_EMULATED_DISPLAY_NAME "Emulated Device API" CACHE INTERNAL "")
set(CPACK_COMPONENT_DEVAPI_EMULATED_DEPENDS calx_ctrl_lib CACHE INTERNAL "")
set(CPACK_COMPONENT_DEVAPI_EMULATED_GROUP "Device API" CACHE INTERNAL "")
set(CPACK_COMPONENT_CONFIG_DESCRIPTION "Minimal Device API implementation. Used for development and porting." CACHE INTERNAL "")