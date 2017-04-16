cmake_minimum_required(VERSION 2.6)
project(CalX)

include_directories(${CMAKE_SOURCE_DIR})
add_library(dev_emulated SHARED EmuMotor.cpp EmuInstrument.cpp EmuDeviceManager.cpp)
target_link_libraries(dev_emulated calx)
install(TARGETS dev_emulated DESTINATION lib/calx_devapi)
