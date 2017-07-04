cmake_minimum_required(VERSION 3.1)
project(CalX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if (MINGW AND MINGW_STATIC_LINKAGE)
	set(STATIC_LINKAGE "-static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -Wl,--exclude-symbols,_Unwind_Resume -lwinpthread -Wl,-Bdynamic,--no-whole-archive")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${STATIC_LINKAGE}")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${STATIC_LINKAGE}")
	set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} ${STATIC_LINKAGE}")
	set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${STATIC_LINKAGE}")
endif (MINGW AND MINGW_STATIC_LINKAGE)

set(build_directory ${CMAKE_BINARY_DIR}/build CACHE INTERNAL "")
add_custom_target(make-build-directory ALL COMMAND ${CMAKE_COMMAND} -E make_directory ${build_directory})
add_custom_target(copy-config ALL
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/main.conf.ini ${build_directory}
	# COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/emu.conf.ini ${build_directory}
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/auto.conf ${build_directory}
	DEPENDS make-build-directory
)

add_subdirectory(device)
add_subdirectory(ctrl-lib)
add_subdirectory(cli)
add_subdirectory(ui)
add_subdirectory(ext)
configure_file(${CMAKE_SOURCE_DIR}/misc/main.conf.ini ${CMAKE_BINARY_DIR}/main.conf.ini COPYONLY)
# configure_file(${CMAKE_SOURCE_DIR}/misc/emu.conf.ini ${CMAKE_BINARY_DIR}/emu.conf.ini COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/misc/auto.conf ${CMAKE_BINARY_DIR}/auto.conf COPYONLY)
if (WIN32)
	install(FILES ${CMAKE_BINARY_DIR}/main.conf.ini ${CMAKE_BINARY_DIR}/auto.conf DESTINATION / COMPONENT config)
else (WIN32)
	install(FILES ${CMAKE_BINARY_DIR}/main.conf.ini ${CMAKE_BINARY_DIR}/auto.conf DESTINATION /etc/calx COMPONENT config)
endif (WIN32)
set(CPACK_COMPONENT_CONFIG_DISPLAY_NAME "Configuration files" CACHE INTERNAL "")
set(CPACK_COMPONENT_CONFIG_DESCRIPTION "Set of default configuration files" CACHE INTERNAL "")


# CPack
set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "CalX")
set(CPACK_PACKAGE_VENDOR "Jevgenijs Protopopovs")
set(CPACK_PACKAGE_CONTACT "jprotopopov1122@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Stepper motor and instrument tool set")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "9")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "CalX")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING.LESSER")
if (WIN32)
	if (EXISTS ${CMAKE_BINARY_DIR}/icon.ico)
		set(CPACK_NSIS_MUI_ICON ${CMAKE_BINARY_DIR}/icon.ico)
		set(CPACK_NSIS_MUI_UNIICON ${CMAKE_BINARY_DIR}/icon.ico)
	endif (EXISTS ${CMAKE_BINARY_DIR}/icon.ico)

	list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "CreateShortCut \\\"$SMPROGRAMS\\\\CalX\\\\CalX.lnk\\\" \\\"$INSTDIR\\\\calx_ui.exe\\\"")
	list(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "Delete \\\"$SMPROGRAMS\\\\CalX\\\\CalX.lnk\\\"")
endif (WIN32)
include(CPack)
