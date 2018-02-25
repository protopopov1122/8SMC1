cmake_minimum_required(VERSION 3.1)
project(CalX)

set(C_STANDART 99)

if (EXISTS ${CMAKE_SOURCE_DIR}/res/lua)
	if (MSVC)
		add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	endif ()
	file(GLOB LUA_SRC ${CMAKE_SOURCE_DIR}/res/lua/*.c)
	list(REMOVE_ITEM LUA_SRC ${CMAKE_SOURCE_DIR}/res/lua/lua.c ${CMAKE_SOURCE_DIR}/res/lua/luac.c)
	SET_SOURCE_FILES_PROPERTIES( ${LUA_SRC} PROPERTIES LANGUAGE CXX )
	if (CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter -Wno-unused-variable")
	endif ()
	if (MINGW AND MINGW_STATIC_LINKAGE)
		set(CMAKE_STATIC_LINKER_FLAGS "")
	endif ()
	if (EXISTS ${CMAKE_SOURCE_DIR}/res/selene)
		file(GLOB SRC *.cpp)
		include_directories(${CMAKE_SOURCE_DIR}/headers ${CMAKE_SOURCE_DIR}/res/lua ${CMAKE_SOURCE_DIR}/res/selene)
		add_library(lua-calx SHARED ${SRC} ${LUA_SRC})
		target_link_libraries(lua-calx calx)
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
	else ()
		message(WARNING "Selene headers not found! Lua bindigs can't be compiled")
	endif ()
else ()
	message(WARNING "Lua sources not found! Lua bindings can't be compiled")
endif()