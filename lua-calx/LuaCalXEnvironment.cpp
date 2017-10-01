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

#include "lua-calx/LuaCalXEnvironment.h"

namespace CalXLua {

	LuaCalXEnvironment::LuaCalXEnvironment(CalXScriptEnvironment &env)
	    : env(env) {}

	bool LuaCalXEnvironment::connectSerialMotor(int port, int baudrate,
	                                            int parity) {
		return env.connectSerialMotor(port, baudrate, parity);
	}

	bool LuaCalXEnvironment::connectSerialInstrument(int port, int baudrate,
	                                                 int parity) {
		return env.connectSerialInstrument(port, baudrate, parity);
	}

	int LuaCalXEnvironment::getMotorCount() {
		return env.getMotorCount();
	}

	int LuaCalXEnvironment::getInstrumentCount() {
		return env.getInstrumentCount();
	}

	int LuaCalXEnvironment::getMotorPower(int id) {
		std::pair<Power, ErrorCode> res = env.getMotorPower(static_cast<device_id_t>(id));
		return static_cast<int>(res.first);
	}

	int LuaCalXEnvironment::enableMotorPower(int id, bool power) {
		ErrorCode res = env.enableMotorPower(static_cast<device_id_t>(id), power);
		return static_cast<int>(res);
	}

	int LuaCalXEnvironment::motorMove(int id, int pos, double speed) {
		ErrorCode res = env.motorMove(
		    static_cast<device_id_t>(id), static_cast<motor_coord_t>(pos), speed);
		return static_cast<int>(res);
	}

	int LuaCalXEnvironment::motorRelativeMove(int id, int pos, double speed) {
		ErrorCode res = env.motorRelativeMove(
		    static_cast<device_id_t>(id), static_cast<motor_coord_t>(pos), speed);
		return static_cast<int>(res);
	}

	int LuaCalXEnvironment::motorStop(int id) {
		ErrorCode res = env.motorStop(static_cast<device_id_t>(id));
		return static_cast<int>(res);
	}

	int LuaCalXEnvironment::motorPosition(int id) {
		std::pair<motor_coord_t, ErrorCode> res = env.getMotorPosition(static_cast<device_id_t>(id));
		return static_cast<int>(res.first);
	}

	int LuaCalXEnvironment::motorMoveToTrailer(int id, int tr) {
		ErrorCode res = env.motorMoveToTrailer(static_cast<device_id_t>(id),
		                                               static_cast<TrailerId>(tr));
		return static_cast<int>(res);
	}

	bool LuaCalXEnvironment::motorCheckTrailers(int id) {
		std::pair<bool, ErrorCode> res = env.motorCheckTrailers(static_cast<device_id_t>(id));
		return res.first;
	}

	int LuaCalXEnvironment::motorWaitWhileRunning(int id) {
		ErrorCode res = env.motorWaitWhileRunning(static_cast<device_id_t>(id));
		return static_cast<int>(res);
	}
	
	int LuaCalXEnvironment::instrumentOpenSession(int id) {
		ErrorCode res = env.instrumentOpenSession(static_cast<device_id_t>(id));
		return static_cast<int>(res);
	}
	
	int LuaCalXEnvironment::instrumentCloseSession(int id) {
		ErrorCode res = env.instrumentCloseSession(static_cast<device_id_t>(id));
		return static_cast<int>(res);
	}
	
	int LuaCalXEnvironment::instrumentEnable(int id, bool en) {
		ErrorCode res = env.instrumentEnable(static_cast<device_id_t>(id), en);
		return static_cast<int>(res);
	}
	
	bool LuaCalXEnvironment::instrumentIsEnabled(int id) {
		std::pair<bool, ErrorCode> res = env.instrumentIsEnabled(static_cast<device_id_t>(id));
		return res.first;
	}
	
	int LuaCalXEnvironment::instrumentSetRunnable(int id, bool r) {
		ErrorCode res = env.instrumentSetRunnable(static_cast<device_id_t>(id), r);
		return static_cast<int>(res);
	}
	
	bool LuaCalXEnvironment::instrumentIsRunnable(int id) {
		std::pair<bool, ErrorCode> res = env.instrumentIsRunnable(static_cast<device_id_t>(id));
		return res.first;
	}
	
	int LuaCalXEnvironment::instrumentGetMode(int id) {
		std::pair<InstrumentMode, ErrorCode> res = env.instrumentGetMode(static_cast<device_id_t>(id));
		return static_cast<int>(res.first);
	}
	
	bool LuaCalXEnvironment::instrumentSetMode(int id, int mode) {
		std::pair<bool, ErrorCode> res = env.instrumentSetMode(static_cast<device_id_t>(id), static_cast<InstrumentMode>(mode));
		return res.first;
	}
	
	bool LuaCalXEnvironment::instrumentIsSessionOpened(int id) {
		std::pair<bool, ErrorCode> res = env.instrumentIsSessionOpened(static_cast<device_id_t>(id));
		return res.first;
	}
	
	std::string LuaCalXEnvironment::instrumentGetInfo(int id) {
		std::pair<std::string, ErrorCode> res = env.instrumentGetInfo(static_cast<device_id_t>(id));
		return res.first;
	}
}  // namespace CalXLua
