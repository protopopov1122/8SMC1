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

#include "lua-calx/LuaScriptEngine.h"

namespace CalXLua {

	LuaCalXScript::LuaCalXScript(CalXScriptEnvironment &env)
	    : CalXScript(env), lua(true), lua_env(env) {
		this->lua["calx"].SetObj(lua_env, "connectSerialMotor",
		                         &LuaCalXEnvironment::connectSerialMotor,
		                         "connectSerialInstrument",
		                         &LuaCalXEnvironment::connectSerialInstrument,
								 "motorCount", &LuaCalXEnvironment::getMotorCount,
								 "instrumentCount", &LuaCalXEnvironment::getInstrumentCount);
		this->lua.Load(env.getConfiguration()->getEntry("script")->getString(
		    "main", "scripts/main.lua"));
	}

	void LuaCalXScript::call(std::string hook) {
		this->lua[hook.c_str()]();
	}
}  // namespace CalXLua
