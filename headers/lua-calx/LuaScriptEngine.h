/*
        Copyright (c) 2017-2018 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License as published
   by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CALX_LUA_CALX_LUA_SCRIPT_ENGINE_H_
#define CALX_LUA_CALX_LUA_SCRIPT_ENGINE_H_

#include "ctrl-lib/script/ScriptEngine.h"
#include "lua-calx/LuaCalXEnvironment.h"

using namespace CalX;

namespace CalXLua {

	class LuaCalXScript : public CalXScript {
	 public:
		LuaCalXScript(CalXScriptEnvironment &, std::string);

		bool execute(std::string) override;
		bool call(std::string) override;

	 private:
		void bind_functions();
		void init_constants();

		lcb::LuaEnvironment lua;
		LuaCalXEnvironment lua_env;
	};

	class LuaCalXScriptFactory : public CalXScriptFactory {
	 public:
		virtual std::unique_ptr<CalXScript> openFile(CalXScriptEnvironment &,
		                                             std::string);
		virtual std::unique_ptr<CalXScript> createShell(CalXScriptEnvironment &);
	};
}  // namespace CalXLua

#endif
