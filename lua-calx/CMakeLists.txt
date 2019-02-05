cmake_minimum_required(VERSION 3.1)
project(CalX)

set(LUACPPB_FPIC ON)
set(LUA_VERSION 5.3.5)
set(LUACPPB_COMPILE_LUA ON)
set(LUACPPB_LUA_SOURCE ${CMAKE_SOURCE_DIR}/third-party/lua-${LUA_VERSION})
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/third-party/LuaCppB/cmake/")
find_package(LuaCppB REQUIRED)

if (LUACPPB)
  file(GLOB SRC *.cpp)
  add_library(lua-calx SHARED ${SRC})
  target_include_directories(lua-calx PUBLIC ${CMAKE_SOURCE_DIR}/headers ${LUACPPB_LUA_HEADERS} ${LUACPPB_HEADERS})
  target_link_libraries(lua-calx calx luacppb luacppb_lua)
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
  add_custom_command(TARGET lua-calx POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CONFIG_DIR}/scripts ${build_directory}/scripts
  )
endif (LUACPPB)