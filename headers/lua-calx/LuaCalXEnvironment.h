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
namespace lcb = LuaCppB;

namespace CalXLua {

	class CalXException : public std::exception {
	 public:
		CalXException(ErrorCode);
		ErrorCode getErrorCode();
		virtual const char *what() const throw();

	 private:
		ErrorCode errcode;
	};

	class LuaCalXMotor {
	 public:
		LuaCalXMotor(CalXScriptUIEnvironment &, device_id_t);

		device_id_t getDeviceID() const;
		Power getPower();
		void enablePower(bool);
		void move(motor_coord_t, float);
		void relativeMove(motor_coord_t, float);
		void stop();
		motor_coord_t getPosition();
		void moveToTrailer(TrailerId);
		void checkTrailers();
		void waitWhileRunning();
	 private:
		std::unique_ptr<CalXScriptMotor> motor;
		device_id_t deviceId;
	};

	class LuaCalXMotors {
	 public:
	 	LuaCalXMotors(CalXScriptUIEnvironment &);
		std::unique_ptr<LuaCalXMotor> connectSerialMotor(uint8_t, uint32_t, SerialPortParity);
		std::size_t getCount();
		std::unique_ptr<LuaCalXMotor> getMotor(device_id_t);
	 private:
		CalXScriptUIEnvironment &env;
	};

	class LuaCalXInstrument {
	 public:
		LuaCalXInstrument(CalXScriptUIEnvironment &, device_id_t);
	
		device_id_t getDeviceID() const;
		void open_session();
		void close_session();
		void enable(bool);
		bool isEnabled();
		bool isRunnable();
		void setRunnable(bool);
		InstrumentMode getMode();
		void setMode(InstrumentMode);
		bool isSessionOpened();
		std::string getInfo();
	 private:
		std::unique_ptr<CalXScriptInstrument> instrument;
		device_id_t deviceId;
	};

	class LuaCalXInstruments {
	 public:
		LuaCalXInstruments(CalXScriptUIEnvironment &);
		std::unique_ptr<LuaCalXInstrument> connectSerialInstrument(uint8_t, uint32_t, SerialPortParity);
		std::size_t getCount();
		std::unique_ptr<LuaCalXInstrument> getInstrument(device_id_t);
	 private:
		CalXScriptUIEnvironment &env;
	};

	class LuaCalXPlane {
	 public:
		LuaCalXPlane(CalXScriptUIEnvironment &, std::size_t);
		std::size_t getPlaneID() const;
		void move(coord_point_t, double, bool, bool);
		void arc(coord_point_t, coord_point_t, int, double, bool,
		                      bool);
		void calibrate(TrailerId);
		void measure(TrailerId);
		void move(coord_point_t, double);
		void configure(coord_point_t, double);
		coord_point_t getPosition();
		coord_rect_t getSize();
		bool isMeasured();
		bool positionAsCenter();
		void openWatcher();
	 private:
	 	CalXScriptUIEnvironment &env;
		std::unique_ptr<CalXScriptPlane> plane;
		std::size_t planeId;
	};

	class LuaCalXPlanes {
	 public:
		LuaCalXPlanes(CalXScriptUIEnvironment &);
		std::unique_ptr<LuaCalXPlane> create(LuaCalXMotor &, LuaCalXMotor &, LuaCalXInstrument &);
		std::unique_ptr<LuaCalXPlane> getPlane(std::size_t);
	 private:
	 	CalXScriptUIEnvironment &env;
	};

	class LuaCalXConfig {
	 public:
		LuaCalXConfig(CalXScriptUIEnvironment &);
		ConfiguationFlatDictionary &getEntry(const std::string &);
		bool hasEntry(const std::string &);
	 private:
		CalXScriptUIEnvironment &env;
	};

	class LuaCalXSettings {
	 public:
		LuaCalXSettings(CalXScriptUIEnvironment &);
		bool exists();
		ConfiguationFlatDictionary &getEntry(const std::string &);
		bool hasEntry(const std::string &);
	 private:
		CalXScriptUIEnvironment &env;
	};
}  // namespace CalXLua

#endif
