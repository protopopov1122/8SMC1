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
		return static_cast<int>(env.getMotorPower(static_cast<device_id_t>(id)));
	}

	int LuaCalXEnvironment::enableMotorPower(int id, bool power) {
		return static_cast<int>(
		    env.enableMotorPower(static_cast<device_id_t>(id), power));
	}

	int LuaCalXEnvironment::motorMove(int id, int pos, double speed) {
		return static_cast<int>(env.motorMove(
		    static_cast<device_id_t>(id), static_cast<motor_coord_t>(pos), speed));
	}

	int LuaCalXEnvironment::motorRelativeMove(int id, int pos, double speed) {
		return static_cast<int>(env.motorRelativeMove(
		    static_cast<device_id_t>(id), static_cast<motor_coord_t>(pos), speed));
	}

	int LuaCalXEnvironment::motorStop(int id) {
		return static_cast<int>(env.motorStop(static_cast<device_id_t>(id)));
	}

	int LuaCalXEnvironment::motorPosition(int id) {
		return static_cast<int>(env.getMotorPosition(static_cast<device_id_t>(id)));
	}

	int LuaCalXEnvironment::motorMoveToTrailer(int id, int tr) {
		return static_cast<int>(env.motorMoveToTrailer(static_cast<device_id_t>(id),
		                                               static_cast<TrailerId>(tr)));
	}

	bool LuaCalXEnvironment::motorCheckTrailers(int id) {
		return env.motorCheckTrailers(static_cast<device_id_t>(id));
	}

	int LuaCalXEnvironment::motorWaitWhileRunning(int id) {
		return static_cast<int>(
		    env.motorWaitWhileRunning(static_cast<device_id_t>(id)));
	}
	
	int LuaCalXEnvironment::instrumentOpenSession(int id) {
		return static_cast<int>(env.instrumentOpenSession(static_cast<device_id_t>(id)));
	}
	
	int LuaCalXEnvironment::instrumentCloseSession(int id) {
		return static_cast<int>(env.instrumentCloseSession(static_cast<device_id_t>(id)));
	}
	
	int LuaCalXEnvironment::instrumentEnable(int id, bool en) {
		return static_cast<int>(env.instrumentEnable(static_cast<device_id_t>(id), en));
	}
	
	bool LuaCalXEnvironment::instrumentIsEnabled(int id) {
		return env.instrumentIsEnabled(static_cast<device_id_t>(id));
	}
	
	void LuaCalXEnvironment::instrumentSetRunnable(int id, bool r) {
		env.instrumentSetRunnable(static_cast<device_id_t>(id), r);
	}
	
	bool LuaCalXEnvironment::instrumentIsRunnable(int id) {
		return env.instrumentIsRunnable(static_cast<device_id_t>(id));
	}
	
	int LuaCalXEnvironment::instrumentGetMode(int id) {
		return static_cast<int>(env.instrumentGetMode(static_cast<device_id_t>(id)));
	}
	
	bool LuaCalXEnvironment::instrumentSetMode(int id, int mode) {
		return env.instrumentSetMode(static_cast<device_id_t>(id), static_cast<InstrumentMode>(mode));
	}
	
	bool LuaCalXEnvironment::instrumentIsSessionOpened(int id) {
		return env.instrumentIsSessionOpened(static_cast<device_id_t>(id));
	}
	
	std::string LuaCalXEnvironment::instrumentGetInfo(int id) {
		return env.instrumentGetInfo(static_cast<device_id_t>(id));
	}
}  // namespace CalXLua
