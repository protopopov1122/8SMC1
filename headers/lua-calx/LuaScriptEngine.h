/*
        Copyright (c) 2017 Jevgenijs Protopopovs

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
#include "selene.h"

using namespace CalX;
using namespace CalXUI;

namespace CalXLua {

	class LuaCalXScript : public CalXScript {
	 public:
		LuaCalXScript(CalXScriptEnvironment &);
		virtual ~LuaCalXScript() = default;

		virtual void call(std::string);

	 private:
		sel::State lua;
		LuaCalXEnvironment lua_env;
	};
}  // namespace CalXLua

#endif
