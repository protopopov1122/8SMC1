cmake_minimum_required(VERSION 2.6)
project(CalX)

add_subdirectory(device)
add_subdirectory(ctrl-lib)
add_subdirectory(ui)
configure_file(${CMAKE_SOURCE_DIR}/config.ini ${CMAKE_BINARY_DIR}/config.ini COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/auto.conf ${CMAKE_BINARY_DIR}/auto.conf COPYONLY)
install(FILES config.ini auto.conf DESTINATION etc/calx)
