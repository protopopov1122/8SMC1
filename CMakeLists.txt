cmake_minimum_required(VERSION 2.6)
project(CalX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_subdirectory(device)
add_subdirectory(ctrl-lib)
add_subdirectory(cli)
add_subdirectory(ui)
configure_file(${CMAKE_SOURCE_DIR}/config.ini ${CMAKE_BINARY_DIR}/config.ini COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/auto.conf ${CMAKE_BINARY_DIR}/auto.conf COPYONLY)
if (WIN32)
	install(FILES config.ini auto.conf DESTINATION /)
else (WIN32)
	install(FILES config.ini auto.conf DESTINATION /etc/calx)
endif (WIN32)

# CPack

set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "CalX")
set(CPACK_PACKAGE_VENDOR "Jevgenijs Protopopovs")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Stepper motor and laser tool set")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "CalX")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
include(CPACK)
