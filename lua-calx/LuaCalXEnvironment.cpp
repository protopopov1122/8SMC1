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
		DeviceSerialPortConnectionPrms prms;
		prms.port = static_cast<uint8_t>(port);
		prms.speed = static_cast<uint32_t>(baudrate);
		prms.parity = static_cast<SerialPortParity>(parity);
		return static_cast<int>(env.getDevices().connectMotor(&prms));
	}

	int LuaCalXEnvironment::connectSerialInstrument(int port, int baudrate,
	                                                int parity) {
		DeviceSerialPortConnectionPrms prms;
		prms.port = static_cast<uint8_t>(port);
		prms.speed = static_cast<uint32_t>(baudrate);
		prms.parity = static_cast<SerialPortParity>(parity);
		return static_cast<int>(env.getDevices().connectInstrument(&prms));
	}

	int LuaCalXEnvironment::getMotorCount() {
		return env.getDevices().getMotorCount();
	}

	int LuaCalXEnvironment::getInstrumentCount() {
		return env.getDevices().getInstrumentCount();
	}

	int LuaCalXEnvironment::getMotorPower(int id) {
		std::optional<Power> res =
		    env.getDevices().getMotor(static_cast<device_id_t>(id))->getPower();
		if (!res.has_value()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		return static_cast<int>(res.value());
	}

	int LuaCalXEnvironment::enableMotorPower(int id, bool power) {
		ErrorCode res = env.getDevices().getMotor(static_cast<device_id_t>(id))->enablePower(power);
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return static_cast<int>(res);
	}

	int LuaCalXEnvironment::motorMove(int id, int pos, double speed) {
		ErrorCode res = env.getDevices().getMotor(static_cast<device_id_t>(id))->move(static_cast<motor_coord_t>(pos), speed);
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return static_cast<int>(res);
	}

	int LuaCalXEnvironment::motorRelativeMove(int id, int pos, double speed) {
		ErrorCode res = env.getDevices().getMotor(static_cast<device_id_t>(id))->relativeMove(static_cast<motor_coord_t>(pos), speed);
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return static_cast<int>(res);
	}

	int LuaCalXEnvironment::motorStop(int id) {
		ErrorCode res = env.getDevices().getMotor(static_cast<device_id_t>(id))->stop();
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return static_cast<int>(res);
	}

	int LuaCalXEnvironment::motorPosition(int id) {
		std::optional<motor_coord_t> res =
		    env.getDevices().getMotor(static_cast<device_id_t>(id))->getPosition();
		if (!res.has_value()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		return static_cast<int>(res.value());
	}

	int LuaCalXEnvironment::motorMoveToTrailer(int id, int tr) {
		ErrorCode res = env.getDevices().getMotor(static_cast<device_id_t>(id))->moveToTrailer(static_cast<TrailerId>(tr));
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return static_cast<int>(res);
	}

	bool LuaCalXEnvironment::motorCheckTrailers(int id) {
		ErrorCode res =
		    env.getDevices().getMotor(static_cast<device_id_t>(id))->checkTrailers();
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return true;
	}

	int LuaCalXEnvironment::motorWaitWhileRunning(int id) {
		ErrorCode res = env.getDevices().getMotor(static_cast<device_id_t>(id))->waitWhileRunning();
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return static_cast<int>(res);
	}

	int LuaCalXEnvironment::instrumentOpenSession(int id) {
		ErrorCode res = env.getDevices().getInstrument(static_cast<device_id_t>(id))->open_session();
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return static_cast<int>(res);
	}

	int LuaCalXEnvironment::instrumentCloseSession(int id) {
		ErrorCode res = env.getDevices().getInstrument(static_cast<device_id_t>(id))->close_session();
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return static_cast<int>(res);
	}

	int LuaCalXEnvironment::instrumentEnable(int id, bool en) {
		ErrorCode res = env.getDevices().getInstrument(static_cast<device_id_t>(id))->enable(en);
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return static_cast<int>(res);
	}

	bool LuaCalXEnvironment::instrumentIsEnabled(int id) {
		std::optional<bool> res =
		    env.getDevices().getInstrument(static_cast<device_id_t>(id))->isEnabled();
		if (!res.has_value()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		return res.value();
	}

	int LuaCalXEnvironment::instrumentSetRunnable(int id, bool r) {
		ErrorCode res = env.getDevices().getInstrument(static_cast<device_id_t>(id))->setRunnable(r);
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return static_cast<int>(res);
	}

	bool LuaCalXEnvironment::instrumentIsRunnable(int id) {
		std::optional<bool> res =
		    env.getDevices().getInstrument(static_cast<device_id_t>(id))->isRunnable();
		if (!res.has_value()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		return res.value();
	}

	int LuaCalXEnvironment::instrumentGetMode(int id) {
		std::optional<InstrumentMode> res =
		    env.getDevices().getInstrument(static_cast<device_id_t>(id))->getMode();
		if (!res.has_value()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		return static_cast<int>(res.value());
	}

	bool LuaCalXEnvironment::instrumentSetMode(int id, int mode) {
		ErrorCode res = env.getDevices().getInstrument(static_cast<device_id_t>(id))->setMode(
		    static_cast<InstrumentMode>(mode));
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return true;
	}

	bool LuaCalXEnvironment::instrumentIsSessionOpened(int id) {
		std::optional<bool> res =
		    env.getDevices().getInstrument(static_cast<device_id_t>(id))->isSessionOpened();
		if (!res.has_value()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		return res.value();
	}

	std::string LuaCalXEnvironment::instrumentGetInfo(int id) {
		std::optional<std::string> res =
		    env.getDevices().getInstrument(static_cast<device_id_t>(id))->getInfo();
		if (!res.has_value()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		return res.value();
	}

	int LuaCalXEnvironment::planeCreate(int m1, int m2, int ins) {
		return static_cast<int>(env.getPlanes().createPlane(
		    static_cast<device_id_t>(m1), static_cast<device_id_t>(m2),
		    static_cast<device_id_t>(ins)));
	}

	int LuaCalXEnvironment::planeMove(int id, double x, double y, double speed,
	                                  bool sync, bool relative) {
		coord_point_t dest = { x, y };
		ErrorCode errcode =
		    env.getPlanes().getPlane(static_cast<std::size_t>(id))->move(dest, speed, sync, relative);
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
		ErrorCode errcode = env.getPlanes().getPlane(static_cast<std::size_t>(id))->arc(dest, cen,
		                                 splitter, speed, clockwise, relative);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
		return static_cast<int>(errcode);
	}

	int LuaCalXEnvironment::planeCalibrate(int id, int tid) {
		ErrorCode errcode = env.getPlanes().getPlane(static_cast<std::size_t>(id))->calibrate(static_cast<TrailerId>(tid));
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
		return static_cast<int>(errcode);
	}

	int LuaCalXEnvironment::planeMeasure(int id, int tid) {
		ErrorCode errcode =
		    env.getPlanes().getPlane(static_cast<std::size_t>(id))->measure(static_cast<TrailerId>(tid));
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
		return static_cast<int>(errcode);
	}

	int LuaCalXEnvironment::planeFMove(int id, double x, double y, double speed) {
		coord_point_t dest = { x, y };
		ErrorCode errcode = env.getPlanes().getPlane(static_cast<std::size_t>(id))->move(dest, speed);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
		return static_cast<int>(errcode);
	}

	int LuaCalXEnvironment::planeConfigure(int id, double x, double y,
	                                       double speed) {
		coord_point_t dest = { x, y };
		ErrorCode errcode =
		    env.getPlanes().getPlane(static_cast<std::size_t>(id))->configure(dest, speed);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
		return static_cast<int>(errcode);
	}

	int LuaCalXEnvironment::planeNewWatcher(int id) {
		ErrorCode errcode = env.getPlanes().getPlane(static_cast<std::size_t>(id))->newWatcher();
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
		return static_cast<int>(errcode);
	}

	double LuaCalXEnvironment::planeGetPositionX(int id) {
		std::optional<coord_point_t> res =
		    env.getPlanes().getPlane(static_cast<std::size_t>(id))->getPosition();
		if (!res.has_value()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		return res.value().x;
	}

	double LuaCalXEnvironment::planeGetPositionY(int id) {
		std::optional<coord_point_t> res =
		    env.getPlanes().getPlane(static_cast<std::size_t>(id))->getPosition();
		if (!res.has_value()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		return res.value().y;
	}

	double LuaCalXEnvironment::planeGetSizeX(int id) {
		std::pair<coord_rect_t, ErrorCode> res =
		    env.getPlanes().getPlane(static_cast<size_t>(id))->getSize();
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first.x;
	}

	double LuaCalXEnvironment::planeGetSizeY(int id) {
		std::pair<coord_rect_t, ErrorCode> res =
		    env.getPlanes().getPlane(static_cast<size_t>(id))->getSize();
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first.y;
	}

	double LuaCalXEnvironment::planeGetSizeW(int id) {
		std::pair<coord_rect_t, ErrorCode> res =
		    env.getPlanes().getPlane(static_cast<size_t>(id))->getSize();
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first.w;
	}

	double LuaCalXEnvironment::planeGetSizeH(int id) {
		std::pair<coord_rect_t, ErrorCode> res =
		    env.getPlanes().getPlane(static_cast<size_t>(id))->getSize();
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first.h;
	}

	bool LuaCalXEnvironment::planeIsMeasured(int id) {
		std::optional<bool> res =
		    env.getPlanes().getPlane(static_cast<size_t>(id))->isMeasured();
		if (!res.has_value()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		return res.value();
	}

	int LuaCalXEnvironment::planePositionAsCenter(int id) {
		bool res = env.getPlanes().getPlane(static_cast<size_t>(id))->positionAsCenter();
		if (res) {
			return static_cast<int>(ErrorCode::NoError);
		} else {
			throw CalXException(ErrorCode::UnknownResource);
		}
	}
}  // namespace CalXLua
