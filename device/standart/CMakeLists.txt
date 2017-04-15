cmake_minimum_required(VERSION 2.6)
project(CalX)

if (WIN32)
	include_directories(${CMAKE_SOURCE_DIR})
	add_library(dev_standart SHARED 8SMC1Device.cpp NL300Instrument.cpp StandartDeviceManager.cpp)
	install(TARGETS dev_standart DESTINATION lib/calx_devapi)
endif (WIN32)
