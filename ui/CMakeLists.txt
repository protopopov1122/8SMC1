cmake_minimum_required(VERSION 3.1)
project(CalX)

set(wxWidgets_USE_LIBS)
if (WIN32)
	if (CMAKE_BUILD_TYPE EQUAL DEBUG)
		set(wxWidgets_CONFIGURATION  mswud)
	else ()
		set(wxWidgets_CONFIGURATION  mswu)
	endif ()
else (WIN32)
	set(wxWidgets_USE_UNICODE ON)
	if (CMAKE_BUILD_TYPE EQUAL DEBUG)
		set(wxWidgets_USE_DEBUG ON)
	else ()
		set(wxWidgets_USE_DEBUG OFF)
	endif ()
endif (WIN32)
find_package(wxWidgets COMPONENTS adv core base)

if (wxWidgets_FOUND)
	file(GLOB_RECURSE SRCS *.cpp ${CMAKE_SOURCE_DIR}/cli/CLI.cpp ${CMAKE_SOURCE_DIR}/cli/DevCLI.cpp)
	if (EXISTS ${CMAKE_SOURCE_DIR}/misc/icon.ico)
		configure_file(${CMAKE_SOURCE_DIR}/misc/icon.ico ${build_directory}/icon.ico COPYONLY)
		install(FILES ${build_directory}/icon.ico DESTINATION /)
		set(SRCS ${SRCS} calxui.rc)
	endif (EXISTS ${CMAKE_SOURCE_DIR}/misc/icon.ico)
	add_executable(calx_ui ${SRCS})
	include(${wxWidgets_USE_FILE})
	target_include_directories(calx_ui PUBLIC ${CMAKE_SOURCE_DIR}/headers)
	target_include_directories(calx_ui PUBLIC ${CMAKE_SOURCE_DIR})
	if(MSVC)
		set_target_properties(calx_ui PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
	endif (MSVC)
	target_link_libraries(calx_ui ${wxWidgets_LIBRARIES} calx)
	add_custom_command(TARGET calx_ui POST_BUILD
    	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:calx_ui> ${build_directory}
	)
	add_custom_command(TARGET calx_ui POST_BUILD
    	COMMAND ${CMAKE_COMMAND} -E copy ${CONFIG_DIR}/ui.ini ${build_directory}
	)
	add_dependencies(calx_ui calx-init-target)
	if (WIN32)
		target_link_libraries(calx_ui uxtheme)
		install(TARGETS calx_ui DESTINATION / COMPONENT calx_ui)
	else (WIN32)
		install(TARGETS calx_ui DESTINATION bin COMPONENT calx_ui)
	endif (WIN32)
	set(CPACK_COMPONENT_CALX_UI_DISPLAY_NAME "CalX User interface" CACHE INTERNAL "")
	set(CPACK_COMPONENT_CALX_UI_DEPENDS calx_ctrl_lib CACHE INTERNAL "")
	set(CPACK_COMPONENT_CALX_UI_GROUP "User interface" CACHE INTERNAL "")
	set(CPACK_COMPONENT_CALX_UI_DESCRIPTION "Graphics User interface." CACHE INTERNAL "")
else(wxWidgets_FOUND)
	message("wxWidgets not found!")
endif(wxWidgets_FOUND)
