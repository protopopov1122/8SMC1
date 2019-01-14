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

#ifndef CALX_LUA_CALX_LUA_CALX_ENVIRONMENT_H_
#define CALX_LUA_CALX_LUA_CALX_ENVIRONMENT_H_

#include "ui/script/ScriptEngine.h"
#include "luacppb/LuaCppB.h"
#include <exception>

using namespace CalX;
using namespace CalXUI;

namespace CalXLua {

	class CalXException : public std::exception {
	 public:
		CalXException(ErrorCode);
		ErrorCode getErrorCode();
		virtual const char *what() const throw();

	 private:
		ErrorCode errcode;
	};

	namespace lcb = LuaCppB;

	class LuaCalXEnvironmentPosition {
	 public:
	 	LuaCalXEnvironmentPosition(CalXScriptUIEnvironment &);
		double planeGetPositionX(std::size_t);
		double planeGetPositionY(std::size_t);
		ErrorCode planePositionAsCenter(std::size_t);
	 private:
	 	CalXScriptUIEnvironment &env;
	};

	class LuaCalXEnvironmentSize {
	 public:
		LuaCalXEnvironmentSize(CalXScriptUIEnvironment &);
		double planeGetSizeX(std::size_t);
		double planeGetSizeY(std::size_t);
		double planeGetSizeW(std::size_t);
		double planeGetSizeH(std::size_t);
	 private:
		CalXScriptUIEnvironment &env;
	};

	class LuaCalXEnvironment {
	 public:
		LuaCalXEnvironment(CalXScriptUIEnvironment &, lcb::LuaState &);

		device_id_t connectSerialMotor(uint8_t, uint32_t, SerialPortParity);
		device_id_t connectSerialInstrument(uint8_t, uint32_t, SerialPortParity);
		std::size_t getMotorCount();
		std::size_t getInstrumentCount();
		Power getMotorPower(device_id_t);
		ErrorCode enableMotorPower(device_id_t, bool);
		ErrorCode motorMove(device_id_t, motor_coord_t, double);
		ErrorCode motorRelativeMove(device_id_t, motor_coord_t, double);
		ErrorCode motorStop(device_id_t);
		motor_coord_t motorPosition(device_id_t);
		ErrorCode motorMoveToTrailer(device_id_t, TrailerId);
		bool motorCheckTrailers(device_id_t);
		ErrorCode motorWaitWhileRunning(device_id_t);

		ErrorCode instrumentOpenSession(device_id_t);
		ErrorCode instrumentCloseSession(device_id_t);
		ErrorCode instrumentEnable(device_id_t, bool);
		bool instrumentIsEnabled(device_id_t);
		ErrorCode instrumentSetRunnable(device_id_t, bool);
		bool instrumentIsRunnable(device_id_t);
		InstrumentMode instrumentGetMode(device_id_t);
		bool instrumentSetMode(device_id_t, InstrumentMode);
		bool instrumentIsSessionOpened(device_id_t);
		std::string instrumentGetInfo(device_id_t);

		std::size_t planeCreate(device_id_t, device_id_t, device_id_t);
		ErrorCode planeMove(std::size_t, double, double, double, bool, bool);
		ErrorCode planeArc(std::size_t, double, double, double, double, int, double, bool, bool);
		ErrorCode planeCalibrate(std::size_t, TrailerId);
		ErrorCode planeMeasure(std::size_t, TrailerId);
		ErrorCode planeFMove(std::size_t, double, double, double);
		ErrorCode planeConfigure(std::size_t, double, double, double);
		ErrorCode planeNewWatcher(std::size_t);
		bool planeIsMeasured(std::size_t);

		int getConfigurationInt(const std::string &, const std::string &);
		double getConfigurationFloat(const std::string &, const std::string &);
		std::string getConfigurationString(const std::string &, const std::string &);
		bool getConfigurationBoolean(const std::string &, const std::string &);
		bool configurationHas(const std::string &, const std::string &);

		bool hasSettings();
		int getSettingsInt(const std::string &, const std::string &);
		double getSettingsFloat(const std::string &, const std::string &);
		std::string getSettingsString(const std::string &, const std::string &);
		bool getSettingsBoolean(const std::string &, const std::string &);
		bool settingsHas(const std::string &, const std::string &);

		lcb::LuaTable MotorPower;
		lcb::LuaTable MotorTrailer;
		lcb::LuaTable InstrMode;

		LuaCalXEnvironmentPosition Position;
		LuaCalXEnvironmentSize Size;
	 private:
		CalXScriptUIEnvironment &env;
	};
}  // namespace CalXLua

#endif
