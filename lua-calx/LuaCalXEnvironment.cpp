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

	const char *CalXException::what() const throw() {
		return "CalX runtime error";
	}

	LuaCalXMotor::LuaCalXMotor(CalXScriptUIEnvironment &env, device_id_t deviceId)
		: motor(env.getDevices().getMotor(deviceId)), deviceId(deviceId) {
		if (!this->motor->isValid()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
	}
	
	device_id_t LuaCalXMotor::getDeviceID() const {
		return this->deviceId;
	}

	Power LuaCalXMotor::getPower() {
		return this->motor->getPower().value();
	}

	void LuaCalXMotor::enablePower(bool en) {
		ErrorCode errcode = this->motor->enablePower(en);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
	}

	void LuaCalXMotor::move(motor_coord_t dest, float speed) {
		ErrorCode errcode = this->motor->move(dest, speed);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
	}

	void LuaCalXMotor::relativeMove(motor_coord_t dest, float speed) {
		ErrorCode errcode = this->motor->relativeMove(dest, speed);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
	}

	void LuaCalXMotor::stop() {
		ErrorCode errcode = this->motor->stop();
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
	}

	motor_coord_t LuaCalXMotor::getPosition() {
		return this->motor->getPosition().value();
	}

	void LuaCalXMotor::moveToTrailer(TrailerId trailer) {
		ErrorCode errcode = this->motor->moveToTrailer(trailer);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
	}

	void LuaCalXMotor::checkTrailers() {
		ErrorCode errcode = this->motor->checkTrailers();
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
	}

	void LuaCalXMotor::waitWhileRunning() {
		ErrorCode errcode = this->motor->waitWhileRunning();
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
	}

	LuaCalXMotors::LuaCalXMotors(CalXScriptUIEnvironment &env)
		: env(env) {}
	
	std::unique_ptr<LuaCalXMotor> LuaCalXMotors::connectSerialMotor(uint8_t port, uint32_t baudrate, SerialPortParity parity) {
		DeviceSerialPortConnectionPrms prms;
		prms.port = port;
		prms.speed = baudrate;
		prms.parity = parity;
		return std::make_unique<LuaCalXMotor>(this->env, this->env.getDevices().connectMotor(&prms));
	}

	std::size_t LuaCalXMotors::getCount() {
		return this->env.getDevices().getMotorCount();
	}
	
	std::unique_ptr<LuaCalXMotor> LuaCalXMotors::getMotor(device_id_t deviceId) {
		return std::make_unique<LuaCalXMotor>(this->env, deviceId);
	}

	LuaCalXInstrument::LuaCalXInstrument(CalXScriptUIEnvironment &env, device_id_t deviceId)
		: instrument(env.getDevices().getInstrument(deviceId)), deviceId(deviceId) {
		if (!this->instrument->isValid()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
	}

	device_id_t LuaCalXInstrument::getDeviceID() const {
		return this->deviceId;
	}

	void LuaCalXInstrument::open_session() {
		ErrorCode errcode = this->instrument->open_session();
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
	}

	void LuaCalXInstrument::close_session() {
		ErrorCode errcode = this->instrument->close_session();
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
	}

	void LuaCalXInstrument::enable(bool en) {
		ErrorCode errcode = this->instrument->enable(en);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
	}

	bool LuaCalXInstrument::isEnabled() {
		return this->instrument->isEnabled().value();
	}

	bool LuaCalXInstrument::isRunnable() {
		return this->instrument->isRunnable().value();
	}

	void LuaCalXInstrument::setRunnable(bool run) {
		ErrorCode errcode = this->instrument->setRunnable(run);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
	}

	InstrumentMode LuaCalXInstrument::getMode() {
		return this->instrument->getMode().value();
	}

	void LuaCalXInstrument::setMode(InstrumentMode mode) {
		ErrorCode errcode = this->instrument->setMode(mode);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
	}

	bool LuaCalXInstrument::isSessionOpened() {
		return this->instrument->isSessionOpened().value();
	}

	std::string LuaCalXInstrument::getInfo() {
		return this->instrument->getInfo().value();
	}

	LuaCalXInstruments::LuaCalXInstruments(CalXScriptUIEnvironment &env)
		: env(env) {}
	
	std::unique_ptr<LuaCalXInstrument> LuaCalXInstruments::connectSerialInstrument(uint8_t port, uint32_t baudrate, SerialPortParity parity) {
		DeviceSerialPortConnectionPrms prms;
		prms.port = port;
		prms.speed = baudrate;
		prms.parity = parity;
		return std::make_unique<LuaCalXInstrument>(this->env, this->env.getDevices().connectInstrument(&prms));
	}

	std::size_t LuaCalXInstruments::getCount() {
		return this->env.getDevices().getInstrumentCount();
	}

	std::unique_ptr<LuaCalXInstrument> LuaCalXInstruments::getInstrument(device_id_t deviceId) {
		return std::make_unique<LuaCalXInstrument>(this->env, deviceId);
	}

	LuaCalXPlane::LuaCalXPlane(CalXScriptUIEnvironment &env, std::size_t planeId)
		: env(env), plane(env.getPlanes().getPlane(planeId)), planeId(planeId) {
		if (this->plane == nullptr) {
			throw CalXException(ErrorCode::UnknownResource);
		}
	}
	
	std::size_t LuaCalXPlane::getPlaneID() const {
		return this->planeId;
	}

	void LuaCalXPlane::move(coord_point_t dest, double speed, bool sync, bool relative) {
		ErrorCode errcode = this->plane->move(dest, speed, sync, relative);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
	}

	void LuaCalXPlane::arc(coord_point_t dest, coord_point_t center, int splitter, double speed, bool clockwise, bool relative) {
		ErrorCode errcode = this->plane->arc(dest, center, splitter, speed, clockwise, relative);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
	}

	void LuaCalXPlane::calibrate(TrailerId id) {
		ErrorCode errcode = this->plane->calibrate(id);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
	}

	void LuaCalXPlane::measure(TrailerId id) {
		ErrorCode errcode = this->plane->measure(id);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
	}

	void LuaCalXPlane::move(coord_point_t dest, double speed) {
		ErrorCode errcode = this->plane->move(dest, speed);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
	}

	void LuaCalXPlane::configure(coord_point_t dest, double speed) {
		ErrorCode errcode = this->plane->configure(dest, speed);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
	}

	coord_point_t LuaCalXPlane::getPosition() {
		return this->plane->getPosition().value();
	}

	coord_rect_t LuaCalXPlane::getSize() {
		std::pair<coord_rect_t, ErrorCode> res =  this->plane->getSize();
		if (res.second != ErrorCode::NoError) {
			throw CalXException(res.second);
		} else {
			return res.first;
		}
	}

	bool LuaCalXPlane::isMeasured() {
		return this->plane->isMeasured().value();
	}

	bool LuaCalXPlane::positionAsCenter() {
		return this->plane->positionAsCenter();
	}

	void LuaCalXPlane::openWatcher() {
		ErrorCode errcode = this->env.getUI().openWatcher(this->planeId);
		if (errcode != ErrorCode::NoError) {
			throw CalXException(errcode);
		}
	}

	LuaCalXPlanes::LuaCalXPlanes(CalXScriptUIEnvironment &env)
		: env(env) {}

	std::unique_ptr<LuaCalXPlane> LuaCalXPlanes::create(LuaCalXMotor &motor1, LuaCalXMotor &motor2, LuaCalXInstrument &instr) {
		return std::make_unique<LuaCalXPlane>(this->env, this->env.getPlanes().createPlane(motor1.getDeviceID(), motor2.getDeviceID(), instr.getDeviceID()));
	}

	std::unique_ptr<LuaCalXPlane> LuaCalXPlanes::getPlane(std::size_t planeId) {
		return std::make_unique<LuaCalXPlane>(this->env, planeId);
	}

	LuaCalXConfig::LuaCalXConfig(CalXScriptUIEnvironment &env)
		: env(env) {}
	
	ConfiguationFlatDictionary &LuaCalXConfig::getEntry(const std::string &entryName) {
		ConfiguationFlatDictionary *entry = this->env.getConfiguration().getEntry(entryName, false);
		if (entry) {
			return *entry;
		} else {
			throw CalXException(ErrorCode::UnknownResource);
		}
	}

	bool LuaCalXConfig::hasEntry(const std::string &entryName) {
		return this->env.getConfiguration().hasEntry(entryName);
	}

	LuaCalXSettings::LuaCalXSettings(CalXScriptUIEnvironment &env)
		: env(env) {}
	
	bool LuaCalXSettings::exists() {
		return this->env.getSettings() != nullptr;
	}

	ConfiguationFlatDictionary &LuaCalXSettings::getEntry(const std::string &entryName) {
		ConfiguationFlatDictionary *entry = nullptr;
		if (this->env.getSettings() != nullptr) {
			entry = this->env.getSettings()->getSettings().getEntry(entryName, false);
		}
		if (entry) {
			return *entry;
		} else {
			throw CalXException(ErrorCode::UnknownResource);
		}
	}

	bool LuaCalXSettings::hasEntry(const std::string &entryName) {
		return this->env.getSettings() != nullptr &&
			this->env.getSettings()->getSettings().hasEntry(entryName);
	}
}  // namespace CalXLua
