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
		
		this->bind_functions();
		this->init_constants();
		
		this->lua.Load(env.getConfiguration()->getEntry("script")->getString(
		    "main", "scripts/main.lua"));
	}

	void LuaCalXScript::call(std::string hook) {
		this->lua[hook.c_str()]();
	}
	
	void LuaCalXScript::bind_functions() {
		this->lua["calx"].SetObj(lua_env, "connectSerialMotor",
		                         &LuaCalXEnvironment::connectSerialMotor,
		                         "connectSerialInstrument",
		                         &LuaCalXEnvironment::connectSerialInstrument,
								 "motorCount", &LuaCalXEnvironment::getMotorCount,
								 "instrumentCount", &LuaCalXEnvironment::getInstrumentCount,
								 "motorPower", &LuaCalXEnvironment::getMotorPower,
								 "enableMotorPower", &LuaCalXEnvironment::enableMotorPower,
								 "motorMove", &LuaCalXEnvironment::motorMove,
								 "motorRelativeMove", &LuaCalXEnvironment::motorRelativeMove,
								 "motorStop", &LuaCalXEnvironment::motorStop,
								 "motorPosition", &LuaCalXEnvironment::motorPosition,
								 "motorWaitWhileRunning", &LuaCalXEnvironment::motorWaitWhileRunning,
								 "motorMoveToTrailer", &LuaCalXEnvironment::motorMoveToTrailer,
								 "motorCheckTrailers", &LuaCalXEnvironment::motorCheckTrailers);
		
	}
	
	void LuaCalXScript::init_constants() {
		this->lua["calx"]["power"]["No"] = static_cast<int>(Power::NoPower);
		this->lua["calx"]["power"]["Half"] = static_cast<int>(Power::HalfPower);
		this->lua["calx"]["power"]["Full"] = static_cast<int>(Power::FullPower);
		
		this->lua["calx"]["parity"]["No"] = static_cast<int>(SerialPortParity::No);
		this->lua["calx"]["parity"]["Odd"] = static_cast<int>(SerialPortParity::Odd);
		this->lua["calx"]["parity"]["Even"] = static_cast<int>(SerialPortParity::Even);
		this->lua["calx"]["parity"]["Mark"] = static_cast<int>(SerialPortParity::Mark);
		this->lua["calx"]["parity"]["Space"] = static_cast<int>(SerialPortParity::Space);
		
		this->lua["calx"]["trailer"]["Top"] = static_cast<int>(TrailerId::Trailer1);
		this->lua["calx"]["trailer"]["Bottom"] = static_cast<int>(TrailerId::Trailer2);
	}
}  // namespace CalXLua
