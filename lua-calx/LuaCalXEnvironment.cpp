/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

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
		this->halt_on_fail = env.getConfiguration().getEntry("script")->getBool(
		    "halt_on_fail", false);
	}

	device_id_t LuaCalXMotor::getDeviceID() const {
		return this->deviceId;
	}

	std::optional<Power> LuaCalXMotor::getPower() {
		std::optional<Power> power = this->motor->getPower();
		if (power.has_value() || !this->halt_on_fail) {
			return power;
		} else {
			throw CalXException(ErrorCode::UnknownResource);
		}
	}

	ErrorCode LuaCalXMotor::enablePower(bool en) {
		ErrorCode errcode = this->motor->enablePower(en);
		if (errcode != ErrorCode::NoError && this->halt_on_fail) {
			throw CalXException(errcode);
		} else {
			return errcode;
		}
	}

	ErrorCode LuaCalXMotor::move(motor_coord_t dest, float speed) {
		ErrorCode errcode = this->motor->move(dest, speed);
		if (errcode != ErrorCode::NoError && this->halt_on_fail) {
			throw CalXException(errcode);
		} else {
			return errcode;
		}
	}

	ErrorCode LuaCalXMotor::relativeMove(motor_coord_t dest, float speed) {
		ErrorCode errcode = this->motor->relativeMove(dest, speed);
		if (errcode != ErrorCode::NoError && this->halt_on_fail) {
			throw CalXException(errcode);
		} else {
			return errcode;
		}
	}

	ErrorCode LuaCalXMotor::stop() {
		ErrorCode errcode = this->motor->stop();
		if (errcode != ErrorCode::NoError && this->halt_on_fail) {
			throw CalXException(errcode);
		} else {
			return errcode;
		}
	}

	std::optional<motor_coord_t> LuaCalXMotor::getPosition() {
		std::optional<motor_coord_t> position = this->motor->getPosition();
		if (position.has_value() || !this->halt_on_fail) {
			return position;
		} else {
			throw CalXException(ErrorCode::UnknownResource);
		}
	}

	ErrorCode LuaCalXMotor::moveToTrailer(TrailerId trailer) {
		ErrorCode errcode = this->motor->moveToTrailer(trailer);
		if (errcode != ErrorCode::NoError && this->halt_on_fail) {
			throw CalXException(errcode);
		} else {
			return errcode;
		}
	}

	ErrorCode LuaCalXMotor::checkTrailers() {
		ErrorCode errcode = this->motor->checkTrailers();
		if (errcode != ErrorCode::NoError && this->halt_on_fail) {
			throw CalXException(errcode);
		} else {
			return errcode;
		}
	}

	ErrorCode LuaCalXMotor::waitWhileRunning() {
		ErrorCode errcode = this->motor->waitWhileRunning();
		if (errcode != ErrorCode::NoError && this->halt_on_fail) {
			throw CalXException(errcode);
		} else {
			return errcode;
		}
	}

	LuaCalXMotors::LuaCalXMotors(CalXScriptUIEnvironment &env) : env(env) {}

	std::unique_ptr<LuaCalXMotor> LuaCalXMotors::connectSerialMotor(
	    uint8_t port, uint32_t baudrate, SerialPortParity parity) {
		DeviceSerialPortConnectionPrms prms;
		prms.port = port;
		prms.speed = baudrate;
		prms.parity = parity;
		return std::make_unique<LuaCalXMotor>(
		    this->env, this->env.getDevices().connectMotor(&prms));
	}

	std::size_t LuaCalXMotors::getCount() {
		return this->env.getDevices().getMotorCount();
	}

	std::unique_ptr<LuaCalXMotor> LuaCalXMotors::getMotor(device_id_t deviceId) {
		return std::make_unique<LuaCalXMotor>(this->env, deviceId);
	}

	LuaCalXInstrument::LuaCalXInstrument(CalXScriptUIEnvironment &env,
	                                     device_id_t deviceId)
	    : instrument(env.getDevices().getInstrument(deviceId)),
	      deviceId(deviceId) {
		if (!this->instrument->isValid()) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		this->halt_on_fail = env.getConfiguration().getEntry("script")->getBool(
		    "halt_on_fail", false);
	}

	device_id_t LuaCalXInstrument::getDeviceID() const {
		return this->deviceId;
	}

	ErrorCode LuaCalXInstrument::open_session() {
		ErrorCode errcode = this->instrument->open_session();
		if (errcode != ErrorCode::NoError && this->halt_on_fail) {
			throw CalXException(errcode);
		} else {
			return errcode;
		}
	}

	ErrorCode LuaCalXInstrument::close_session() {
		ErrorCode errcode = this->instrument->close_session();
		if (errcode != ErrorCode::NoError && this->halt_on_fail) {
			throw CalXException(errcode);
		} else {
			return errcode;
		}
	}

	ErrorCode LuaCalXInstrument::enable(bool en) {
		ErrorCode errcode = this->instrument->enable(en);
		if (errcode != ErrorCode::NoError && this->halt_on_fail) {
			throw CalXException(errcode);
		} else {
			return errcode;
		}
	}

	std::optional<bool> LuaCalXInstrument::isEnabled() {
		std::optional<bool> value = this->instrument->isEnabled();
		if (value.has_value() || !this->halt_on_fail) {
			return value;
		} else {
			throw CalXException(ErrorCode::UnknownResource);
		}
	}

	std::optional<bool> LuaCalXInstrument::isRunnable() {
		std::optional<bool> value = this->instrument->isRunnable();
		if (value.has_value() || !this->halt_on_fail) {
			return value;
		} else {
			throw CalXException(ErrorCode::UnknownResource);
		}
	}

	ErrorCode LuaCalXInstrument::setRunnable(bool run) {
		ErrorCode errcode = this->instrument->setRunnable(run);
		if (errcode != ErrorCode::NoError && this->halt_on_fail) {
			throw CalXException(errcode);
		} else {
			return errcode;
		}
	}

	std::optional<InstrumentMode> LuaCalXInstrument::getMode() {
		std::optional<InstrumentMode> mode = this->instrument->getMode();
		if (mode.has_value() || !this->halt_on_fail) {
			return mode;
		} else {
			throw CalXException(ErrorCode::UnknownResource);
		}
	}

	ErrorCode LuaCalXInstrument::setMode(InstrumentMode mode) {
		ErrorCode errcode = this->instrument->setMode(mode);
		if (errcode != ErrorCode::NoError && this->halt_on_fail) {
			throw CalXException(errcode);
		} else {
			return errcode;
		}
	}

	std::optional<bool> LuaCalXInstrument::isSessionOpened() {
		std::optional<bool> value = this->instrument->isSessionOpened();
		if (value.has_value() || !this->halt_on_fail) {
			return value;
		} else {
			throw CalXException(ErrorCode::UnknownResource);
		}
	}

	std::optional<std::string> LuaCalXInstrument::getInfo() {
		std::optional<std::string> info = this->instrument->getInfo().value();
		if (info.has_value() || !this->halt_on_fail) {
			return info;
		} else {
			throw CalXException(ErrorCode::UnknownResource);
		}
	}

	LuaCalXInstruments::LuaCalXInstruments(CalXScriptUIEnvironment &env)
	    : env(env) {}

	std::unique_ptr<LuaCalXInstrument>
	    LuaCalXInstruments::connectSerialInstrument(uint8_t port,
	                                                uint32_t baudrate,
	                                                SerialPortParity parity) {
		DeviceSerialPortConnectionPrms prms;
		prms.port = port;
		prms.speed = baudrate;
		prms.parity = parity;
		return std::make_unique<LuaCalXInstrument>(
		    this->env, this->env.getDevices().connectInstrument(&prms));
	}

	std::size_t LuaCalXInstruments::getCount() {
		return this->env.getDevices().getInstrumentCount();
	}

	std::unique_ptr<LuaCalXInstrument> LuaCalXInstruments::getInstrument(
	    device_id_t deviceId) {
		return std::make_unique<LuaCalXInstrument>(this->env, deviceId);
	}

	LuaCalXPlane::LuaCalXPlane(CalXScriptUIEnvironment &env, std::size_t planeId)
	    : env(env), plane(env.getPlanes().getPlane(planeId)), planeId(planeId) {
		if (this->plane == nullptr) {
			throw CalXException(ErrorCode::UnknownResource);
		}
		this->halt_on_fail = env.getConfiguration().getEntry("script")->getBool(
		    "halt_on_fail", false);
	}

	std::size_t LuaCalXPlane::getPlaneID() const {
		return this->planeId;
	}

	ErrorCode LuaCalXPlane::move(coord_point_t dest, double speed, bool sync,
	                             bool relative) {
		ErrorCode errcode = this->plane->move(dest, speed, sync, relative);
		if (errcode != ErrorCode::NoError && this->halt_on_fail) {
			throw CalXException(errcode);
		} else {
			return errcode;
		}
	}

	ErrorCode LuaCalXPlane::arc(coord_point_t dest, coord_point_t center,
	                            int splitter, double speed, bool clockwise,
	                            bool relative) {
		ErrorCode errcode =
		    this->plane->arc(dest, center, splitter, speed, clockwise, relative);
		if (errcode != ErrorCode::NoError && this->halt_on_fail) {
			throw CalXException(errcode);
		} else {
			return errcode;
		}
	}

	ErrorCode LuaCalXPlane::calibrate(TrailerId id) {
		ErrorCode errcode = this->plane->calibrate(id);
		if (errcode != ErrorCode::NoError && this->halt_on_fail) {
			throw CalXException(errcode);
		} else {
			return errcode;
		}
	}

	ErrorCode LuaCalXPlane::measure(TrailerId id) {
		ErrorCode errcode = this->plane->measure(id);
		if (errcode != ErrorCode::NoError && this->halt_on_fail) {
			throw CalXException(errcode);
		} else {
			return errcode;
		}
	}

	ErrorCode LuaCalXPlane::move(coord_point_t dest, double speed) {
		ErrorCode errcode = this->plane->move(dest, speed);
		if (errcode != ErrorCode::NoError && this->halt_on_fail) {
			throw CalXException(errcode);
		} else {
			return errcode;
		}
	}

	ErrorCode LuaCalXPlane::configure(coord_point_t dest, double speed) {
		ErrorCode errcode = this->plane->configure(dest, speed);
		if (errcode != ErrorCode::NoError && this->halt_on_fail) {
			throw CalXException(errcode);
		} else {
			return errcode;
		}
	}

	std::optional<coord_point_t> LuaCalXPlane::getPosition() {
		std::optional<coord_point_t> position = this->plane->getPosition();
		if (position.has_value() || !this->halt_on_fail) {
			return position;
		} else {
			throw CalXException(ErrorCode::UnknownResource);
		}
	}

	std::optional<coord_rect_t> LuaCalXPlane::getSize() {
		std::pair<coord_rect_t, ErrorCode> res = this->plane->getSize();
		if (res.second != ErrorCode::NoError) {
			if (this->halt_on_fail) {
				throw CalXException(res.second);
			} else {
				return std::optional<coord_rect_t>();
			}
		} else {
			return res.first;
		}
	}

	std::optional<bool> LuaCalXPlane::isMeasured() {
		std::optional<bool> value = this->plane->isMeasured();
		if (value.has_value() || !this->halt_on_fail) {
			return value;
		} else {
			throw CalXException(ErrorCode::UnknownResource);
		}
	}

	bool LuaCalXPlane::positionAsCenter() {
		return this->plane->positionAsCenter();
	}

	ErrorCode LuaCalXPlane::openWatcher() {
		ErrorCode errcode = this->env.getUI().openWatcher(this->planeId);
		if (errcode != ErrorCode::NoError && this->halt_on_fail) {
			throw CalXException(errcode);
		} else {
			return errcode;
		}
	}

	LuaCalXPlanes::LuaCalXPlanes(CalXScriptUIEnvironment &env) : env(env) {}

	std::unique_ptr<LuaCalXPlane> LuaCalXPlanes::create(
	    LuaCalXMotor &motor1, LuaCalXMotor &motor2, LuaCalXInstrument &instr) {
		return std::make_unique<LuaCalXPlane>(
		    this->env,
		    this->env.getPlanes().createPlane(
		        motor1.getDeviceID(), motor2.getDeviceID(), instr.getDeviceID()));
	}

	std::size_t LuaCalXPlanes::getPlaneCount() {
		return this->env.getPlanes().getPlaneCount();
	}

	std::unique_ptr<LuaCalXPlane> LuaCalXPlanes::getPlane(std::size_t planeId) {
		return std::make_unique<LuaCalXPlane>(this->env, planeId);
	}

	LuaCalXConfig::LuaCalXConfig(CalXScriptUIEnvironment &env) : env(env) {
		this->halt_on_fail = env.getConfiguration().getEntry("script")->getBool(
		    "halt_on_fail", false);
	}

	std::optional<std::reference_wrapper<ConfiguationFlatDictionary>>
	    LuaCalXConfig::getEntry(const std::string &entryName) {
		ConfiguationFlatDictionary *entry =
		    this->env.getConfiguration().getEntry(entryName, false);
		if (entry) {
			return std::ref(*entry);
		} else if (!this->halt_on_fail) {
			return std::optional<
			    std::reference_wrapper<ConfiguationFlatDictionary>>();
		} else {
			throw CalXException(ErrorCode::UnknownResource);
		}
	}

	bool LuaCalXConfig::hasEntry(const std::string &entryName) {
		return this->env.getConfiguration().hasEntry(entryName);
	}

	LuaCalXSettings::LuaCalXSettings(CalXScriptUIEnvironment &env) : env(env) {
		this->halt_on_fail = env.getConfiguration().getEntry("script")->getBool(
		    "halt_on_fail", false);
	}

	bool LuaCalXSettings::exists() {
		return this->env.getSettings() != nullptr;
	}

	std::optional<std::reference_wrapper<ConfiguationFlatDictionary>>
	    LuaCalXSettings::getEntry(const std::string &entryName) {
		ConfiguationFlatDictionary *entry = nullptr;
		if (this->env.getSettings() != nullptr) {
			entry = this->env.getSettings()->getSettings().getEntry(entryName, false);
		}
		if (entry) {
			return *entry;
		} else if (!this->halt_on_fail) {
			return std::optional<
			    std::reference_wrapper<ConfiguationFlatDictionary>>();
		} else {
			throw CalXException(ErrorCode::UnknownResource);
		}
	}

	bool LuaCalXSettings::hasEntry(const std::string &entryName) {
		return this->env.getSettings() != nullptr &&
		       this->env.getSettings()->getSettings().hasEntry(entryName);
	}

	LuaCalXMath::LuaCalXMath(CalXScriptUIEnvironment &env)
		: env(env) {}
	
	std::size_t LuaCalXMath::getFormulaCount() {
		return this->env.getUI().getMath().getFormulaCount();
	}

	void LuaCalXMath::addFormula(const std::string &title, const std::string &formula, lcb::LuaReferenceHandle variablesRef) {
		std::map<std::string, std::string> variables;
		for (std::size_t i = 1; variablesRef[i].exists(); i++) {
			auto varRef = variablesRef[i];
			variables[varRef["name"].get<std::string>()] = varRef["description"].get<std::string>();
		}
		this->env.getUI().getMath().addFormula(title, formula, variables);
	}

	bool LuaCalXMath::removeFormula(std::size_t index) {
		return this->env.getUI().getMath().removeFormula(index);
	}


	LuaCalXJournal::LuaCalXJournal(JournalLogger &logger)
		: logger(logger) {}
	
	void LuaCalXJournal::debug(const std::string &msg) {
		this->logger.stream(LoggingSeverity::Debug) << msg << Flush();
	}

	void LuaCalXJournal::info(const std::string &msg) {
		this->logger.stream(LoggingSeverity::Info) << msg << Flush();
	}
	
	void LuaCalXJournal::warning(const std::string &msg) {
		this->logger.stream(LoggingSeverity::Warning) << msg << Flush();
	}

	void LuaCalXJournal::error(const std::string &msg) {
		this->logger.stream(LoggingSeverity::Error) << msg << Flush();
	}

	void LuaCalXJournal::critical(const std::string &msg) {
		this->logger.stream(LoggingSeverity::Critical) << msg << Flush();
	}
}  // namespace CalXLua
