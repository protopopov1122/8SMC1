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

	CalXException::CalXException(ErrorCode errcode) : errcode(errcode) {}

	ErrorCode CalXException::getErrorCode() {
		return this->errcode;
	}

	const char* CalXException::what() const throw() {
		return "CalX runtime error";
	}

	LuaCalXEnvironment::LuaCalXEnvironment(CalXScriptEnvironment& env)
	    : env(env) {}

	int LuaCalXEnvironment::connectSerialMotor(int port, int baudrate,
	                                           int parity) {
		return static_cast<int>(env.connectSerialMotor(port, baudrate, parity));
	}

	int LuaCalXEnvironment::connectSerialInstrument(int port, int baudrate,
	                                                int parity) {
		return static_cast<int>(
		    env.connectSerialInstrument(port, baudrate, parity));
	}

	int LuaCalXEnvironment::getMotorCount() {
		return env.getMotorCount();
	}

	int LuaCalXEnvironment::getInstrumentCount() {
		return env.getInstrumentCount();
	}

	int LuaCalXEnvironment::getMotorPower(int id) {
		std::pair<Power, ErrorCode> res =
		    env.getMotorPower(static_cast<device_id_t>(id));
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return static_cast<int>(res.first);
	}

	int LuaCalXEnvironment::enableMotorPower(int id, bool power) {
		ErrorCode res = env.enableMotorPower(static_cast<device_id_t>(id), power);
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return static_cast<int>(res);
	}

	int LuaCalXEnvironment::motorMove(int id, int pos, double speed) {
		ErrorCode res = env.motorMove(static_cast<device_id_t>(id),
		                              static_cast<motor_coord_t>(pos), speed);
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return static_cast<int>(res);
	}

	int LuaCalXEnvironment::motorRelativeMove(int id, int pos, double speed) {
		ErrorCode res = env.motorRelativeMove(
		    static_cast<device_id_t>(id), static_cast<motor_coord_t>(pos), speed);
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return static_cast<int>(res);
	}

	int LuaCalXEnvironment::motorStop(int id) {
		ErrorCode res = env.motorStop(static_cast<device_id_t>(id));
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return static_cast<int>(res);
	}

	int LuaCalXEnvironment::motorPosition(int id) {
		std::pair<motor_coord_t, ErrorCode> res =
		    env.getMotorPosition(static_cast<device_id_t>(id));
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return static_cast<int>(res.first);
	}

	int LuaCalXEnvironment::motorMoveToTrailer(int id, int tr) {
		ErrorCode res = env.motorMoveToTrailer(static_cast<device_id_t>(id),
		                                       static_cast<TrailerId>(tr));
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return static_cast<int>(res);
	}

	bool LuaCalXEnvironment::motorCheckTrailers(int id) {
		std::pair<bool, ErrorCode> res =
		    env.motorCheckTrailers(static_cast<device_id_t>(id));
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first;
	}

	int LuaCalXEnvironment::motorWaitWhileRunning(int id) {
		ErrorCode res = env.motorWaitWhileRunning(static_cast<device_id_t>(id));
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return static_cast<int>(res);
	}

	int LuaCalXEnvironment::instrumentOpenSession(int id) {
		ErrorCode res = env.instrumentOpenSession(static_cast<device_id_t>(id));
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return static_cast<int>(res);
	}

	int LuaCalXEnvironment::instrumentCloseSession(int id) {
		ErrorCode res = env.instrumentCloseSession(static_cast<device_id_t>(id));
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return static_cast<int>(res);
	}

	int LuaCalXEnvironment::instrumentEnable(int id, bool en) {
		ErrorCode res = env.instrumentEnable(static_cast<device_id_t>(id), en);
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return static_cast<int>(res);
	}

	bool LuaCalXEnvironment::instrumentIsEnabled(int id) {
		std::pair<bool, ErrorCode> res =
		    env.instrumentIsEnabled(static_cast<device_id_t>(id));
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first;
	}

	int LuaCalXEnvironment::instrumentSetRunnable(int id, bool r) {
		ErrorCode res = env.instrumentSetRunnable(static_cast<device_id_t>(id), r);
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return static_cast<int>(res);
	}

	bool LuaCalXEnvironment::instrumentIsRunnable(int id) {
		std::pair<bool, ErrorCode> res =
		    env.instrumentIsRunnable(static_cast<device_id_t>(id));
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first;
	}

	int LuaCalXEnvironment::instrumentGetMode(int id) {
		std::pair<InstrumentMode, ErrorCode> res =
		    env.instrumentGetMode(static_cast<device_id_t>(id));
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return static_cast<int>(res.first);
	}

	bool LuaCalXEnvironment::instrumentSetMode(int id, int mode) {
		std::pair<bool, ErrorCode> res = env.instrumentSetMode(
		    static_cast<device_id_t>(id), static_cast<InstrumentMode>(mode));
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first;
	}

	bool LuaCalXEnvironment::instrumentIsSessionOpened(int id) {
		std::pair<bool, ErrorCode> res =
		    env.instrumentIsSessionOpened(static_cast<device_id_t>(id));
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first;
	}

	std::string LuaCalXEnvironment::instrumentGetInfo(int id) {
		std::pair<std::string, ErrorCode> res =
		    env.instrumentGetInfo(static_cast<device_id_t>(id));
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first;
	}

	int LuaCalXEnvironment::planeCreate(int m1, int m2, int ins) {
		return static_cast<int>(env.createCoordPlane(
		    static_cast<device_id_t>(m1), static_cast<device_id_t>(m2),
		    static_cast<device_id_t>(ins)));
	}

	int LuaCalXEnvironment::planeMove(int id, double x, double y, double speed,
	                                  bool sync, bool relative) {
		coord_point_t dest = { x, y };
		ErrorCode errcode =
		    env.planeMove(static_cast<size_t>(id), dest, speed, sync, relative);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
		return static_cast<int>(errcode);
	}

	int LuaCalXEnvironment::planeArc(int id, double x, double y, double cx,
	                                 double cy, int splitter, double speed,
	                                 bool clockwise, bool relative) {
		coord_point_t dest = { x, y };
		coord_point_t cen = { cx, cy };
		ErrorCode errcode = env.planeArc(static_cast<size_t>(id), dest, cen,
		                                 splitter, speed, clockwise, relative);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
		return static_cast<int>(errcode);
	}

	int LuaCalXEnvironment::planeCalibrate(int id, int tid) {
		ErrorCode errcode = env.planeCalibrate(static_cast<size_t>(id),
		                                       static_cast<TrailerId>(tid));
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
		return static_cast<int>(errcode);
	}

	int LuaCalXEnvironment::planeMeasure(int id, int tid) {
		ErrorCode errcode =
		    env.planeMeasure(static_cast<size_t>(id), static_cast<TrailerId>(tid));
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
		return static_cast<int>(errcode);
	}

	int LuaCalXEnvironment::planeFMove(int id, double x, double y, double speed) {
		coord_point_t dest = { x, y };
		ErrorCode errcode = env.planeMove(static_cast<size_t>(id), dest, speed);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
		return static_cast<int>(errcode);
	}

	int LuaCalXEnvironment::planeConfigure(int id, double x, double y,
	                                       double speed) {
		coord_point_t dest = { x, y };
		ErrorCode errcode =
		    env.planeConfigure(static_cast<size_t>(id), dest, speed);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
		return static_cast<int>(errcode);
	}

	int LuaCalXEnvironment::planeNewWatcher(int id) {
		ErrorCode errcode = env.planeNewWatcher(static_cast<size_t>(id));
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
		return static_cast<int>(errcode);
	}

	double LuaCalXEnvironment::planeGetPositionX(int id) {
		std::pair<coord_point_t, ErrorCode> res = env.planeGetPosition(static_cast<size_t>(id));
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first.x;
	}

	double LuaCalXEnvironment::planeGetPositionY(int id) {
		std::pair<coord_point_t, ErrorCode> res = env.planeGetPosition(static_cast<size_t>(id));
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first.y;
	}

	double LuaCalXEnvironment::planeGetSizeX(int id) {
		std::pair<coord_rect_t, ErrorCode> res = env.planeGetSize(static_cast<size_t>(id));
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first.x;
	}

	double LuaCalXEnvironment::planeGetSizeY(int id) {
		std::pair<coord_rect_t, ErrorCode> res = env.planeGetSize(static_cast<size_t>(id));
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first.y;
	}

	double LuaCalXEnvironment::planeGetSizeW(int id) {
		std::pair<coord_rect_t, ErrorCode> res = env.planeGetSize(static_cast<size_t>(id));
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first.w;
	}

	double LuaCalXEnvironment::planeGetSizeH(int id) {
		std::pair<coord_rect_t, ErrorCode> res = env.planeGetSize(static_cast<size_t>(id));
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first.h;
	}

	bool LuaCalXEnvironment::planeIsMeasured(int id) {
		std::pair<bool, ErrorCode> res = env.planeIsMeasured(static_cast<size_t>(id));
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first;
	}
}  // namespace CalXLua
