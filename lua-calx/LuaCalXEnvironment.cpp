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

	device_id_t LuaCalXEnvironment::connectSerialMotor(uint8_t port, uint32_t baudrate,
	                                            SerialPortParity parity) {
		DeviceSerialPortConnectionPrms prms;
		prms.port = port;
		prms.speed = baudrate;
		prms.parity = parity;
		return env.getDevices().connectMotor(&prms);
	}

	device_id_t LuaCalXEnvironment::connectSerialInstrument(uint8_t port, uint32_t baudrate,
	                                           		 	SerialPortParity parity) {
		DeviceSerialPortConnectionPrms prms;
		prms.port = port;
		prms.speed = baudrate;
		prms.parity = parity;
		return env.getDevices().connectInstrument(&prms);
	}

	std::size_t LuaCalXEnvironment::getMotorCount() {
		return env.getDevices().getMotorCount();
	}

	std::size_t LuaCalXEnvironment::getInstrumentCount() {
		return env.getDevices().getInstrumentCount();
	}

	Power LuaCalXEnvironment::getMotorPower(device_id_t id) {
		std::optional<Power> res =
		    env.getDevices().getMotor(id)->getPower();
		if (!res.has_value()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		return res.value();
	}

	ErrorCode LuaCalXEnvironment::enableMotorPower(device_id_t id, bool power) {
		ErrorCode res = env.getDevices()
		                    .getMotor(id)
		                    ->enablePower(power);
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return res;
	}

	ErrorCode LuaCalXEnvironment::motorMove(device_id_t id, motor_coord_t pos, double speed) {
		ErrorCode res = env.getDevices()
		                    .getMotor(id)
		                    ->move(pos, speed);
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return res;
	}

	ErrorCode LuaCalXEnvironment::motorRelativeMove(device_id_t id, motor_coord_t pos, double speed) {
		ErrorCode res = env.getDevices()
		                    .getMotor(id)
		                    ->relativeMove(pos, speed);
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return res;
	}

	ErrorCode LuaCalXEnvironment::motorStop(device_id_t id) {
		ErrorCode res =
		    env.getDevices().getMotor(id)->stop();
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return res;
	}

	motor_coord_t LuaCalXEnvironment::motorPosition(device_id_t id) {
		std::optional<motor_coord_t> res =
		    env.getDevices().getMotor(id)->getPosition();
		if (!res.has_value()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		return res.value();
	}

	ErrorCode LuaCalXEnvironment::motorMoveToTrailer(device_id_t id, TrailerId tr) {
		ErrorCode res = env.getDevices()
		                    .getMotor(id)
		                    ->moveToTrailer(tr);
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return res;
	}

	bool LuaCalXEnvironment::motorCheckTrailers(device_id_t id) {
		ErrorCode res = env.getDevices()
		                    .getMotor(id)
		                    ->checkTrailers();
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return true;
	}

	ErrorCode LuaCalXEnvironment::motorWaitWhileRunning(device_id_t id) {
		ErrorCode res = env.getDevices()
		                    .getMotor(id)
		                    ->waitWhileRunning();
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return res;
	}

	ErrorCode LuaCalXEnvironment::instrumentOpenSession(device_id_t id) {
		ErrorCode res = env.getDevices()
		                    .getInstrument(id)
		                    ->open_session();
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return res;
	}

	ErrorCode LuaCalXEnvironment::instrumentCloseSession(device_id_t id) {
		ErrorCode res = env.getDevices()
		                    .getInstrument(id)
		                    ->close_session();
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return res;
	}

	ErrorCode LuaCalXEnvironment::instrumentEnable(device_id_t id, bool en) {
		ErrorCode res = env.getDevices()
		                    .getInstrument(id)
		                    ->enable(en);
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return res;
	}

	bool LuaCalXEnvironment::instrumentIsEnabled(device_id_t id) {
		std::optional<bool> res = env.getDevices()
		                              .getInstrument(id)
		                              ->isEnabled();
		if (!res.has_value()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		return res.value();
	}

	ErrorCode LuaCalXEnvironment::instrumentSetRunnable(device_id_t id, bool r) {
		ErrorCode res = env.getDevices()
		                    .getInstrument(id)
		                    ->setRunnable(r);
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return res;
	}

	bool LuaCalXEnvironment::instrumentIsRunnable(device_id_t id) {
		std::optional<bool> res = env.getDevices()
		                              .getInstrument(id)
		                              ->isRunnable();
		if (!res.has_value()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		return res.value();
	}

	InstrumentMode LuaCalXEnvironment::instrumentGetMode(device_id_t id) {
		std::optional<InstrumentMode> res =
		    env.getDevices().getInstrument(id)->getMode();
		if (!res.has_value()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		return res.value();
	}

	bool LuaCalXEnvironment::instrumentSetMode(device_id_t id, InstrumentMode mode) {
		ErrorCode res = env.getDevices()
		                    .getInstrument(id)
		                    ->setMode(mode);
		if (res != ErrorCode::NoError) {
			throw CalXException(res);
		}
		return true;
	}

	bool LuaCalXEnvironment::instrumentIsSessionOpened(device_id_t id) {
		std::optional<bool> res = env.getDevices()
		                              .getInstrument(id)
		                              ->isSessionOpened();
		if (!res.has_value()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		return res.value();
	}

	std::string LuaCalXEnvironment::instrumentGetInfo(device_id_t id) {
		std::optional<std::string> res =
		    env.getDevices().getInstrument(id)->getInfo();
		if (!res.has_value()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		return res.value();
	}

	std::size_t LuaCalXEnvironment::planeCreate(device_id_t m1, device_id_t m2, device_id_t ins) {
		return env.getPlanes().createPlane(m1, m2, ins);
	}

	ErrorCode LuaCalXEnvironment::planeMove(std::size_t id, double x, double y, double speed,
	                                  bool sync, bool relative) {
		coord_point_t dest = { x, y };
		ErrorCode errcode = env.getPlanes()
		                        .getPlane(id)
		                        ->move(dest, speed, sync, relative);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
		return errcode;
	}

	ErrorCode LuaCalXEnvironment::planeArc(std::size_t id, double x, double y, double cx,
	                                 double cy, int splitter, double speed,
	                                 bool clockwise, bool relative) {
		coord_point_t dest = { x, y };
		coord_point_t cen = { cx, cy };
		ErrorCode errcode =
		    env.getPlanes()
		        .getPlane(id)
		        ->arc(dest, cen, splitter, speed, clockwise, relative);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
		return errcode;
	}

	ErrorCode LuaCalXEnvironment::planeCalibrate(std::size_t id, TrailerId tid) {
		ErrorCode errcode = env.getPlanes()
		                        .getPlane(id)
		                        ->calibrate(tid);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
		return errcode;
	}

	ErrorCode LuaCalXEnvironment::planeMeasure(std::size_t id, TrailerId tid) {
		ErrorCode errcode = env.getPlanes()
		                        .getPlane(id)
		                        ->measure(tid);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
		return errcode;
	}

	ErrorCode LuaCalXEnvironment::planeFMove(std::size_t id, double x, double y, double speed) {
		coord_point_t dest = { x, y };
		ErrorCode errcode = env.getPlanes()
		                        .getPlane(id)
		                        ->move(dest, speed);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
		return errcode;
	}

	ErrorCode LuaCalXEnvironment::planeConfigure(std::size_t id, double x, double y,
	                                       double speed) {
		coord_point_t dest = { x, y };
		ErrorCode errcode = env.getPlanes()
		                        .getPlane(id)
		                        ->configure(dest, speed);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
		return errcode;
	}

	ErrorCode LuaCalXEnvironment::planeNewWatcher(std::size_t id) {
		ErrorCode errcode =
		    env.getPlanes().getPlane(id)->newWatcher();
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
		return errcode;
	}

	double LuaCalXEnvironment::planeGetPositionX(std::size_t id) {
		std::optional<coord_point_t> res =
		    env.getPlanes().getPlane(id)->getPosition();
		if (!res.has_value()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		return res.value().x;
	}

	double LuaCalXEnvironment::planeGetPositionY(std::size_t id) {
		std::optional<coord_point_t> res =
		    env.getPlanes().getPlane(id)->getPosition();
		if (!res.has_value()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		return res.value().y;
	}

	double LuaCalXEnvironment::planeGetSizeX(std::size_t id) {
		std::pair<coord_rect_t, ErrorCode> res =
		    env.getPlanes().getPlane(id)->getSize();
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first.x;
	}

	double LuaCalXEnvironment::planeGetSizeY(std::size_t id) {
		std::pair<coord_rect_t, ErrorCode> res =
		    env.getPlanes().getPlane(id)->getSize();
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first.y;
	}

	double LuaCalXEnvironment::planeGetSizeW(std::size_t id) {
		std::pair<coord_rect_t, ErrorCode> res =
		    env.getPlanes().getPlane(id)->getSize();
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first.w;
	}

	double LuaCalXEnvironment::planeGetSizeH(std::size_t id) {
		std::pair<coord_rect_t, ErrorCode> res =
		    env.getPlanes().getPlane(id)->getSize();
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		}
		return res.first.h;
	}

	bool LuaCalXEnvironment::planeIsMeasured(std::size_t id) {
		std::optional<bool> res =
		    env.getPlanes().getPlane(id)->isMeasured();
		if (!res.has_value()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		return res.value();
	}

	ErrorCode LuaCalXEnvironment::planePositionAsCenter(std::size_t id) {
		bool res =
		    env.getPlanes().getPlane(static_cast<size_t>(id))->positionAsCenter();
		if (res) {
			return ErrorCode::NoError;
		} else {
			throw CalXException(ErrorCode::UnknownResource);
		}
	}

	int LuaCalXEnvironment::getConfigurationInt(const std::string &entry,
	                                            const std::string &key) {
		return this->env.getConfiguration().getEntry(entry)->getInt(key);
	}

	double LuaCalXEnvironment::getConfigurationFloat(const std::string &entry,
	                                                 const std::string &key) {
		return this->env.getConfiguration().getEntry(entry)->getReal(key);
	}

	std::string LuaCalXEnvironment::getConfigurationString(const std::string &entry,
	                                                       const std::string &key) {
		return this->env.getConfiguration().getEntry(entry)->getString(key);
	}

	bool LuaCalXEnvironment::getConfigurationBoolean(const std::string &entry,
	                                                 const std::string &key) {
		return this->env.getConfiguration().getEntry(entry)->getBool(key);
	}

	bool LuaCalXEnvironment::configurationHas(const std::string &entry,
	                                          const std::string &key) {
		return this->env.getConfiguration().hasEntry(entry) &&
		       this->env.getConfiguration().getEntry(entry)->has(key);
	}

	bool LuaCalXEnvironment::hasSettings() {
		return this->env.getSettings() != nullptr;
	}

	int LuaCalXEnvironment::getSettingsInt(const std::string &entry, const std::string &key) {
		if (this->env.getSettings() != nullptr) {
			return this->env.getSettings()->getSettings().getEntry(entry)->getInt(
			    key);
		} else {
			return 0;
		}
	}

	double LuaCalXEnvironment::getSettingsFloat(const std::string &entry,
	                                            const std::string &key) {
		if (this->env.getSettings() != nullptr) {
			return this->env.getSettings()->getSettings().getEntry(entry)->getReal(
			    key);
		} else {
			return 0.0;
		}
	}

	std::string LuaCalXEnvironment::getSettingsString(const std::string &entry,
	                                                  const std::string &key) {
		if (this->env.getSettings() != nullptr) {
			return this->env.getSettings()->getSettings().getEntry(entry)->getString(
			    key);
		} else {
			return "";
		}
	}
	bool LuaCalXEnvironment::getSettingsBoolean(const std::string &entry,
	                                            const std::string &key) {
		if (this->env.getSettings() != nullptr) {
			return this->env.getSettings()->getSettings().getEntry(entry)->getBool(
			    key);
		} else {
			return false;
		}
	}
	bool LuaCalXEnvironment::settingsHas(const std::string &entry, const std::string &key) {
		return this->env.getSettings() != nullptr &&
		       this->env.getSettings()->getSettings().hasEntry(entry) &&
		       this->env.getSettings()->getSettings().getEntry(entry)->has(key);
	}
}  // namespace CalXLua
