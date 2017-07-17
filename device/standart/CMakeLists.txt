cmake_minimum_required(VERSION 3.1)
project(CalX)

if (WIN32)

	if (EXISTS ${CMAKE_SOURCE_DIR}/res/USMCDLL.h)
		include_directories(${CMAKE_SOURCE_DIR}/res)
		add_library(USMCDLL SHARED stub.cpp)
		if (MINGW)
			SET_TARGET_PROPERTIES(USMCDLL PROPERTIES PREFIX "")
		endif (MINGW)
		add_custom_command(TARGET USMCDLL POST_BUILD
    		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:USMCDLL> ${build_directory}
		)
		add_dependencies(USMCDLL calx-init-target)
		install(TARGETS USMCDLL DESTINATION / COMPONENT usmc_stub)
		set(CPACK_COMPONENT_USMC_STUB_DISPLAY_NAME "USMC driver stub" CACHE INTERNAL "")
		set(CPACK_COMPONENT_USMC_STUB_GROUP "Libraries" CACHE INTERNAL "")
		set(CPACK_COMPONENT_USMC_STUB_DISABLED TRUE CACHE INTERNAL "")
		set(CPACK_COMPONENT_USMC_STUB_DESCRIPTION "Standa 8SMC1 motor emulator. Install only if you haven't installed 8SMC1 motor drivers." CACHE INTERNAL "")
	else (EXISTS ${CMAKE_SOURCE_DIR}/res/USMCDLL.h)
		add_library(USMCDLL SHARED IMPORTED)
	endif (EXISTS ${CMAKE_SOURCE_DIR}/res/USMCDLL.h)


	include_directories(${CMAKE_SOURCE_DIR}/headers)
	add_library(dev_standart SHARED 8SMC1Device.cpp NL300Instrument.cpp NL300Config.cpp NL300Command.cpp NL300Message.cpp StandartDeviceManager.cpp)
	target_link_libraries(dev_standart calx USMCDLL)
	if (MINGW)
		SET_TARGET_PROPERTIES(dev_standart PROPERTIES PREFIX "")
	endif (MINGW)
	add_custom_command(TARGET dev_standart POST_BUILD
    	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:dev_standart> ${build_directory}
    	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/device/standart/NL300.ini ${build_directory}
    	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/device/standart/8SMC1.ini ${build_directory}
	)
	add_dependencies(dev_standart calx-init-target)
#	configure_file(NL300.ini ${CMAKE_BINARY_DIR}/NL300.ini COPYONLY)
#	configure_file(8SMC1.ini ${CMAKE_BINARY_DIR}/8SMC1.ini COPYONLY)
	install(TARGETS dev_standart DESTINATION / COMPONENT devapi_standart)
	install(FILES ${build_directory}/NL300.ini DESTINATION / COMPONENT config)
	install(FILES ${build_directory}/8SMC1.ini DESTINATION / COMPONENT config)
	set(CPACK_COMPONENT_DEVAPI_STANDART_DISPLAY_NAME "8SMC1 stepped motors and NL300 laser Device API" CACHE INTERNAL "")
	set(CPACK_COMPONENT_DEVAPI_STANDART_DEPENDS calx_ctrl_lib CACHE INTERNAL "")
	set(CPACK_COMPONENT_DEVAPI_STANDART_GROUP "Device API" CACHE INTERNAL "")
	set(CPACK_COMPONENT_DEVAPI_STANDART_DESCRIPTION "Standart Device API implementation. Contains Standa 8SMC1 stepper motor and EKSPLA NL300 laser drivers." CACHE INTERNAL "")
endif (WIN32)
