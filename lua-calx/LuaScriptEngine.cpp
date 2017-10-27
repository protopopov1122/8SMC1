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

#include <exception>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "selene.h"
#include "lua-calx/LuaScriptEngine.h"

namespace CalXLua {
	
	std::unique_ptr<CalXScript> LuaCalXScriptFactory::create(CalXScriptEnvironment &env, std::string path) {
		return std::make_unique<LuaCalXScript>(env, path);
	}

	LuaCalXScript::LuaCalXScript(CalXScriptEnvironment &env, std::string path)
	    : CalXScript(env), lua(true), lua_env(env) {
		this->lua.HandleExceptionsWith(
		    [](int s, std::string msg, std::exception_ptr exception) {
			    if (exception) {
				    std::rethrow_exception(exception);
			    }
		    });

		this->bind_functions();
		this->init_constants();

		this->lua.Load(path);
	}

	void LuaCalXScript::call(std::string hook) {
		try {
			this->lua[hook.c_str()]();
		} catch (CalXException &ex) {
			std::cout << "Caught CalX error " << static_cast<int>(ex.getErrorCode())
			          << std::endl;
		}
	}

	void LuaCalXScript::bind_functions() {
		this->lua["calx"]["motor"].SetObj(
		    lua_env, "connectSerial", &LuaCalXEnvironment::connectSerialMotor,
		    "getCount", &LuaCalXEnvironment::getMotorCount, "getPower",
		    &LuaCalXEnvironment::getMotorPower, "enablePower",
		    &LuaCalXEnvironment::enableMotorPower, "move",
		    &LuaCalXEnvironment::motorMove, "relativeMove",
		    &LuaCalXEnvironment::motorRelativeMove, "stop",
		    &LuaCalXEnvironment::motorStop, "getPosition",
		    &LuaCalXEnvironment::motorPosition, "waitWhileRunning",
		    &LuaCalXEnvironment::motorWaitWhileRunning, "moveToTrailer",
		    &LuaCalXEnvironment::motorMoveToTrailer, "checkTrailers",
		    &LuaCalXEnvironment::motorCheckTrailers);

		this->lua["calx"]["instrument"].SetObj(
		    lua_env, "connectSerial", &LuaCalXEnvironment::connectSerialInstrument,
		    "getCount", &LuaCalXEnvironment::getInstrumentCount, "openSession",
		    &LuaCalXEnvironment::instrumentOpenSession, "closeSession",
		    &LuaCalXEnvironment::instrumentCloseSession, "enable",
		    &LuaCalXEnvironment::instrumentEnable, "isEnabled",
		    &LuaCalXEnvironment::instrumentIsEnabled, "setRunnable",
		    &LuaCalXEnvironment::instrumentSetRunnable, "isRunnable",
		    &LuaCalXEnvironment::instrumentIsRunnable, "getMode",
		    &LuaCalXEnvironment::instrumentGetMode, "setMode",
		    &LuaCalXEnvironment::instrumentSetMode, "isSessionOpened",
		    &LuaCalXEnvironment::instrumentIsSessionOpened, "getInfo",
		    &LuaCalXEnvironment::instrumentGetInfo);

		this->lua["calx"]["plane"].SetObj(
		    lua_env, "create", &LuaCalXEnvironment::planeCreate, "move",
		    &LuaCalXEnvironment::planeMove, "arc", &LuaCalXEnvironment::planeArc,
			"calibrate", &LuaCalXEnvironment::planeCalibrate,
			"measure", &LuaCalXEnvironment::planeMeasure,
			"fmove", &LuaCalXEnvironment::planeFMove,
			"configure", &LuaCalXEnvironment::planeConfigure,
			"newWatcher", &LuaCalXEnvironment::planeNewWatcher);
	}

	void LuaCalXScript::init_constants() {
		this->lua["calx"]["motor"]["power"]["No"] =
		    static_cast<int>(Power::NoPower);
		this->lua["calx"]["motor"]["power"]["Half"] =
		    static_cast<int>(Power::HalfPower);
		this->lua["calx"]["motor"]["power"]["Full"] =
		    static_cast<int>(Power::FullPower);

		this->lua["calx"]["motor"]["trailer"]["Top"] =
		    static_cast<int>(TrailerId::Trailer1);
		this->lua["calx"]["motor"]["trailer"]["Bottom"] =
		    static_cast<int>(TrailerId::Trailer2);

		this->lua["calx"]["instrument"]["mode"]["Off"] =
		    static_cast<int>(InstrumentMode::Off);
		this->lua["calx"]["instrument"]["mode"]["Prepare"] =
		    static_cast<int>(InstrumentMode::Prepare);
		this->lua["calx"]["instrument"]["mode"]["Full"] =
		    static_cast<int>(InstrumentMode::Full);

		this->lua["calx"]["serial"]["parity"]["No"] =
		    static_cast<int>(SerialPortParity::No);
		this->lua["calx"]["serial"]["parity"]["Odd"] =
		    static_cast<int>(SerialPortParity::Odd);
		this->lua["calx"]["serial"]["parity"]["Even"] =
		    static_cast<int>(SerialPortParity::Even);
		this->lua["calx"]["serial"]["parity"]["Mark"] =
		    static_cast<int>(SerialPortParity::Mark);
		this->lua["calx"]["serial"]["parity"]["Space"] =
		    static_cast<int>(SerialPortParity::Space);
	}
}  // namespace CalXLua
