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

#include <exception>
#include "lua-calx/LuaScriptEngine.h"
#include <iostream>

namespace CalXLua {

	std::unique_ptr<CalXScript> LuaCalXScriptFactory::openFile(
	    CalXScriptUIEnvironment &env, const std::string &path) {
		return std::make_unique<LuaCalXScript>(env, path);
	}

	std::unique_ptr<CalXScript> LuaCalXScriptFactory::createShell(
	    CalXScriptUIEnvironment &env) {
		return std::make_unique<LuaCalXScript>(env, "");
	}

	LuaCalXScript::LuaCalXScript(CalXScriptUIEnvironment &env, const std::string &path)
	    : CalXScript(env), lua(true), lua_env(env, lua) {
		this->bind_functions();
		this->init_constants();

		if (!path.empty()) {
			this->lua.load(path);
		}
	}

	bool LuaCalXScript::execute(const std::string &code) {
		try {
			return this->lua.execute(code.c_str()) == lcb::LuaStatusCode::Ok;
		} catch (CalXException &ex) {
			std::cout << "Caught CalX error " << static_cast<int>(ex.getErrorCode())
			          << std::endl;
			return false;
		} catch (...) {
			std::cout << "Caught error" << std::endl;
			return false;
		}
	}

	bool LuaCalXScript::call(const std::string &hook) {
		try {
			return this->lua[hook.c_str()]() != lcb::LuaStatusCode::Ok;
		} catch (CalXException &ex) {
			std::cout << "Caught CalX error " << static_cast<int>(ex.getErrorCode())
			          << std::endl;
			return false;
		} catch (...) {
			std::cout << "Caught error" << std::endl;
			return false;
		}
	}

	void LuaCalXScript::bind_functions() {
		lcb::ClassBinder<LuaCalXEnvironmentPosition>::bind(lua,
			"getX", &LuaCalXEnvironmentPosition::planeGetPositionX, "getY",
		    &LuaCalXEnvironmentPosition::planeGetPositionY, "asCenter",
		    &LuaCalXEnvironmentPosition::planePositionAsCenter);
		
		lcb::ClassBinder<LuaCalXEnvironmentSize>::bind(lua,
			"getX", &LuaCalXEnvironmentSize::planeGetSizeX, "getY",
		    &LuaCalXEnvironmentSize::planeGetSizeY, "getW",
		    &LuaCalXEnvironmentSize::planeGetSizeW, "getH",
		    &LuaCalXEnvironmentSize::planeGetSizeH);

		this->lua["calx"] = *lcb::LuaValueFactory::newTable(lua);
		this->lua["calx"]["motor"] = lcb::ObjectBinder::bind(
		    lua_env, lua,
			"connectSerial", &LuaCalXEnvironment::connectSerialMotor,
		    "getCount", &LuaCalXEnvironment::getMotorCount,
			"getPower", &LuaCalXEnvironment::getMotorPower,
			"enablePower", &LuaCalXEnvironment::enableMotorPower,
			"move", &LuaCalXEnvironment::motorMove, 
			"relativeMove", &LuaCalXEnvironment::motorRelativeMove,
			"stop", &LuaCalXEnvironment::motorStop,
			"getPosition", &LuaCalXEnvironment::motorPosition,
			"waitWhileRunning", &LuaCalXEnvironment::motorWaitWhileRunning,
			"moveToTrailer", &LuaCalXEnvironment::motorMoveToTrailer, 
			"checkTrailers", &LuaCalXEnvironment::motorCheckTrailers,
			"power", &LuaCalXEnvironment::MotorPower,
			"trailer", &LuaCalXEnvironment::MotorTrailer);

		this->lua["calx"]["instrument"] = lcb::ObjectBinder::bind(
		    lua_env, lua,
			"connectSerial", &LuaCalXEnvironment::connectSerialInstrument,
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
		    &LuaCalXEnvironment::instrumentGetInfo,
			"mode", &LuaCalXEnvironment::InstrMode);

		this->lua["calx"]["plane"] = lcb::ObjectBinder::bind(
		    lua_env, lua, "create", &LuaCalXEnvironment::planeCreate, "move",
		    &LuaCalXEnvironment::planeMove, "arc", &LuaCalXEnvironment::planeArc,
		    "calibrate", &LuaCalXEnvironment::planeCalibrate, "measure",
		    &LuaCalXEnvironment::planeMeasure, "fmove",
		    &LuaCalXEnvironment::planeFMove, "configure",
		    &LuaCalXEnvironment::planeConfigure, "newWatcher",
		    &LuaCalXEnvironment::planeNewWatcher, "isMeasured",
		    &LuaCalXEnvironment::planeIsMeasured,
			"position", &LuaCalXEnvironment::Position,
			"size", &LuaCalXEnvironment::Size);

		this->lua["calx"]["config"] = lcb::ObjectBinder::bind(
		    lua_env, lua, "getInt", &LuaCalXEnvironment::getConfigurationInt, "getFloat",
		    &LuaCalXEnvironment::getConfigurationFloat, "getBool",
		    &LuaCalXEnvironment::getConfigurationBoolean, "getString",
		    &LuaCalXEnvironment::getConfigurationString, "has",
		    &LuaCalXEnvironment::configurationHas);

		this->lua["calx"]["settings"] = lcb::ObjectBinder::bind(
		    lua_env, lua, "getInt", &LuaCalXEnvironment::getSettingsInt, "getFloat",
		    &LuaCalXEnvironment::getSettingsFloat, "getBool",
		    &LuaCalXEnvironment::getSettingsBoolean, "getString",
		    &LuaCalXEnvironment::getSettingsString, "has",
		    &LuaCalXEnvironment::settingsHas, "exist",
		    &LuaCalXEnvironment::hasSettings);
	}

	void LuaCalXScript::init_constants() {

		auto parity = lcb::LuaValueFactory::newTable(lua);
		this->lua["calx"]["serial"] = *lcb::LuaValueFactory::newTable(lua);
		this->lua["calx"]["serial"]["parity"] = *parity;
		parity["No"] = SerialPortParity::No;
		parity["Odd"] = SerialPortParity::Odd;
		parity["Even"] = SerialPortParity::Even;
		parity["Mark"] = SerialPortParity::Mark;
		parity["Space"] = SerialPortParity::Space;
	}
}  // namespace CalXLua
