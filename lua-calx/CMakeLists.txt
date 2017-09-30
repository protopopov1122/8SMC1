cmake_minimum_required(VERSION 3.1)
project(CalX)

find_package(Lua)

if (LUA_FOUND AND EXISTS ${CMAKE_SOURCE_DIR}/res/selene)
    file(GLOB SRC *.cpp)
    include_directories(${CMAKE_SOURCE_DIR}/headers ${LUA_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/res/selene)
    add_library(lua-calx SHARED ${SRC})
    target_link_libraries(lua-calx calx ${LUA_LIBRARIES})
    if (MINGW)
        SET_TARGET_PROPERTIES(lua-calx PROPERTIES PREFIX "")
    endif (MINGW)
    add_custom_command(TARGET lua-calx POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:lua-calx> ${build_directory}
    )
    add_custom_command(TARGET lua-calx POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${build_directory}/scripts
    )
    add_custom_command(TARGET lua-calx POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/scripts ${build_directory}/scripts
    )
elseif (NOT EXISTS ${CMAKE_SOURCE_DIR}/res/selene)
    message(WARNING "Selene headers not found! Lua bindings can't be compiled")
else ()
    message(WARNING "Lua libraries not found!")
endif ()
