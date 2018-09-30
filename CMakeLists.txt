cmake_minimum_required(VERSION 3.1)
project(CalX)

# Common language standard and compiler setup
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
elseif(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wno-overloaded-virtual -pedantic")
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Fix linker options on MinGW
if (MINGW)
	if (MINGW_STATIC_LINKAGE)
		set(LINKAGE_OPTS "-static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -Wl,--exclude-symbols,_Unwind_Resume -lwinpthread -Wl,-Bdynamic,--no-whole-archive")
	elseif (MINGW_SHARED_LINKAGE)
		set(LINKAGE_OPTS "-shared-libgcc")
	else ()
		set(LINKAGE_OPTS "")
	endif ()
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINKAGE_OPTS}")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LINKAGE_OPTS}")
	set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${LINKAGE_OPTS}")
endif (MINGW)

# Preparations for build
set (THIRD_PARTY ${CMAKE_SOURCE_DIR}/third-party)
set(build_directory ${CMAKE_BINARY_DIR}/build CACHE INTERNAL "")
add_custom_target(make-build-directory ALL COMMAND ${CMAKE_COMMAND} -E make_directory ${build_directory})
add_custom_target(calx-init-target ALL
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/misc/main.conf.ini ${build_directory}
	DEPENDS make-build-directory
)

# Project submodules
add_subdirectory(ctrl-lib)
add_subdirectory(device)
add_subdirectory(cli)
add_subdirectory(ui)
add_subdirectory(ext)
add_subdirectory(lua-calx)
add_subdirectory(tests)

# CPack configuration
set(CPACK_COMPONENT_CONFIG_DISPLAY_NAME "Configuration files" CACHE INTERNAL "")
set(CPACK_COMPONENT_CONFIG_DESCRIPTION "Set of default configuration files" CACHE INTERNAL "")


set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "CalX")
set(CPACK_PACKAGE_VENDOR "Jevgenijs Protopopovs")
set(CPACK_PACKAGE_CONTACT "jprotopopov1122@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Stepper motor and instrument tool set")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "9")
SET(CPACK_PACKAGE_VERSION_PATCH "8")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "CalX")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING.LESSER")

# Create icon and shortcuts on Windows
if (WIN32)
	if (EXISTS ${CMAKE_BINARY_DIR}/icon.ico)
		set(CPACK_NSIS_MUI_ICON ${CMAKE_BINARY_DIR}/icon.ico)
		set(CPACK_NSIS_MUI_UNIICON ${CMAKE_BINARY_DIR}/icon.ico)
	endif (EXISTS ${CMAKE_BINARY_DIR}/icon.ico)

	list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\CalX.lnk\\\" \\\"$INSTDIR\\\\calx_ui.exe\\\"")
	list(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "Delete \\\"$SMPROGRAMS\\\\$START_MENU\\\\CalX.lnk\\\"")
endif (WIN32)
include(CPack)
