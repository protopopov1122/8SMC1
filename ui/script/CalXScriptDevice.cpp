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

#include "ui/script/CalXScriptEngine.h"
#include "ui/dev/CalxDeviceConstructor.h"
#include "ui/CalxErrorHandler.h"

namespace CalXUI {

	CalXAppScriptMotor::CalXAppScriptMotor(CalxApp &app, device_id_t motor_id)
	    : app(app), motor_id(motor_id) {}

	bool CalXAppScriptMotor::isValid() {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()
		        .getMotorControllerSet()
		        .getDeviceController(this->motor_id)
		        .lock();
		return motor != nullptr;
	}

	std::optional<Power> CalXAppScriptMotor::getPower() {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()
		        .getMotorControllerSet()
		        .getDeviceController(this->motor_id)
		        .lock();
		if (motor == nullptr) {
			this->app.getErrorHandler()->alert(
			    FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), this->motor_id),
			    __("Script: Unknown motor"), wxICON_WARNING);
			return std::optional<Power>();
		} else {
			return motor->getPowerState();
		}
	}

	ErrorCode CalXAppScriptMotor::enablePower(bool power) {
		CalxMotorHandle *motor =
		    this->app.getMainFrame()->getDevicePool()->getMotor(this->motor_id);
		if (motor == nullptr) {
			this->app.getErrorHandler()->alert(
			    FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), this->motor_id),
			    __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			return motor->setPower(power);
		}
	}

	ErrorCode CalXAppScriptMotor::move(motor_coord_t pos, float speed) {
		CalxMotorHandle *motor =
		    this->app.getMainFrame()->getDevicePool()->getMotor(this->motor_id);
		if (motor == nullptr) {
			this->app.getErrorHandler()->alert(
			    FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), this->motor_id),
			    __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			auto res = motor->move(pos, speed, false);
			res.wait();
			if (res.getStatus() == CalxActionStatus::Stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.getError();
			}
		}
	}

	ErrorCode CalXAppScriptMotor::relativeMove(motor_coord_t pos, float speed) {
		CalxMotorHandle *motor =
		    this->app.getMainFrame()->getDevicePool()->getMotor(this->motor_id);
		if (motor == nullptr) {
			this->app.getErrorHandler()->alert(
			    FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), this->motor_id),
			    __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			auto res = motor->move(pos, speed, true);
			res.wait();
			if (res.getStatus() == CalxActionStatus::Stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.getError();
			}
		}
	}

	ErrorCode CalXAppScriptMotor::stop() {
		CalxMotorHandle *motor =
		    this->app.getMainFrame()->getDevicePool()->getMotor(this->motor_id);
		if (motor == nullptr) {
			this->app.getErrorHandler()->alert(
			    FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), this->motor_id),
			    __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			motor->stopMovement();
			return ErrorCode::NoError;
		}
	}

	std::optional<motor_coord_t> CalXAppScriptMotor::getPosition() {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()
		        .getMotorControllerSet()
		        .getDeviceController(this->motor_id)
		        .lock();
		if (motor == nullptr) {
			this->app.getErrorHandler()->alert(
			    FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), this->motor_id),
			    __("Script: Unknown motor"), wxICON_WARNING);
			return std::optional<motor_coord_t>();
		} else {
			return motor->getPosition();
		}
	}

	ErrorCode CalXAppScriptMotor::moveToTrailer(TrailerId tr) {
		CalxMotorHandle *motor =
		    this->app.getMainFrame()->getDevicePool()->getMotor(this->motor_id);
		if (motor == nullptr) {
			this->app.getErrorHandler()->alert(
			    FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), this->motor_id),
			    __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			auto res = motor->roll(tr);
			if (res.getStatus() == CalxActionStatus::Stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.getError();
			}
		}
	}

	ErrorCode CalXAppScriptMotor::checkTrailers() {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()
		        .getMotorControllerSet()
		        .getDeviceController(this->motor_id)
		        .lock();
		if (motor == nullptr) {
			this->app.getErrorHandler()->alert(
			    FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), this->motor_id),
			    __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			return motor->checkTrailers();
		}
	}

	ErrorCode CalXAppScriptMotor::waitWhileRunning() {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()
		        .getMotorControllerSet()
		        .getDeviceController(this->motor_id)
		        .lock();
		if (motor == nullptr) {
			this->app.getErrorHandler()->alert(
			    FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), this->motor_id),
			    __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			return motor->waitWhileRunning();
		}
	}

	CalXAppScriptInstrument::CalXAppScriptInstrument(CalxApp &app,
	                                                 device_id_t instrument_id)
	    : app(app), instrument_id(instrument_id) {}

	bool CalXAppScriptInstrument::isValid() {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()
		        .getInstrumentControllerSet()
		        .getDeviceController(this->instrument_id)
		        .lock();
		return instr != nullptr;
	}

	ErrorCode CalXAppScriptInstrument::open_session() {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()
		        .getInstrumentControllerSet()
		        .getDeviceController(this->instrument_id)
		        .lock();
		if (instr == nullptr) {
			this->app.getErrorHandler()->alert(
			    FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"),
			           this->instrument_id),
			    __("Script: Unknown instrument"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			return instr->open_session();
		}
	}

	ErrorCode CalXAppScriptInstrument::close_session() {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()
		        .getInstrumentControllerSet()
		        .getDeviceController(this->instrument_id)
		        .lock();
		if (instr == nullptr) {
			this->app.getErrorHandler()->alert(
			    FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"),
			           this->instrument_id),
			    __("Script: Unknown instrument"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			return instr->close_session();
		}
	}

	ErrorCode CalXAppScriptInstrument::enable(bool en) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()
		        .getInstrumentControllerSet()
		        .getDeviceController(this->instrument_id)
		        .lock();
		if (instr == nullptr) {
			this->app.getErrorHandler()->alert(
			    FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"),
			           this->instrument_id),
			    __("Script: Unknown instrument"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			return instr->enable(en);
		}
	}

	std::optional<bool> CalXAppScriptInstrument::isEnabled() {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()
		        .getInstrumentControllerSet()
		        .getDeviceController(this->instrument_id)
		        .lock();
		if (instr == nullptr) {
			this->app.getErrorHandler()->alert(
			    FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"),
			           this->instrument_id),
			    __("Script: Unknown instrument"), wxICON_WARNING);
			return std::optional<bool>();
		} else {
			return instr->isEnabled();
		}
	}

	std::optional<bool> CalXAppScriptInstrument::isRunnable() {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()
		        .getInstrumentControllerSet()
		        .getDeviceController(this->instrument_id)
		        .lock();
		if (instr == nullptr) {
			this->app.getErrorHandler()->alert(
			    FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"),
			           this->instrument_id),
			    __("Script: Unknown instrument"), wxICON_WARNING);
			return std::optional<bool>();
		} else {
			return instr->isRunnable();
		}
	}

	ErrorCode CalXAppScriptInstrument::setRunnable(bool en) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()
		        .getInstrumentControllerSet()
		        .getDeviceController(this->instrument_id)
		        .lock();
		if (instr == nullptr) {
			this->app.getErrorHandler()->alert(
			    FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"),
			           this->instrument_id),
			    __("Script: Unknown instrument"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			instr->setRunnable(en);
			return ErrorCode::NoError;
		}
	}

	std::optional<InstrumentMode> CalXAppScriptInstrument::getMode() {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()
		        .getInstrumentControllerSet()
		        .getDeviceController(this->instrument_id)
		        .lock();
		if (instr == nullptr) {
			this->app.getErrorHandler()->alert(
			    FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"),
			           this->instrument_id),
			    __("Script: Unknown instrument"), wxICON_WARNING);
			return std::optional<InstrumentMode>();
		} else {
			return instr->getMode();
		}
	}

	ErrorCode CalXAppScriptInstrument::setMode(InstrumentMode mode) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()
		        .getInstrumentControllerSet()
		        .getDeviceController(this->instrument_id)
		        .lock();
		if (instr == nullptr) {
			this->app.getErrorHandler()->alert(
			    FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"),
			           this->instrument_id),
			    __("Script: Unknown instrument"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			instr->setMode(mode);
			return ErrorCode::NoError;
		}
	}
	std::optional<bool> CalXAppScriptInstrument::isSessionOpened() {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()
		        .getInstrumentControllerSet()
		        .getDeviceController(this->instrument_id)
		        .lock();
		if (instr == nullptr) {
			this->app.getErrorHandler()->alert(
			    FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"),
			           this->instrument_id),
			    __("Script: Unknown instrument"), wxICON_WARNING);
			return std::optional<bool>();
		} else {
			return instr->isSessionOpened();
		}
	}
	std::optional<std::string> CalXAppScriptInstrument::getInfo() {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()
		        .getInstrumentControllerSet()
		        .getDeviceController(this->instrument_id)
		        .lock();
		if (instr == nullptr) {
			this->app.getErrorHandler()->alert(
			    FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"),
			           this->instrument_id),
			    __("Script: Unknown instrument"), wxICON_WARNING);
			return std::optional<std::string>();
		} else {
			return instr->getInfo();
		}
	}

	CalXAppScriptDevices::CalXAppScriptDevices(CalxApp &app) : app(app) {}

	device_id_t CalXAppScriptDevices::connectMotor(DeviceConnectionPrms *prms) {
		SystemManager &sysman = this->app.getSystemManager();
		std::shared_ptr<MotorController> ctrl =
		    sysman.getMotorControllerSet().connectDevice(prms).lock();
		if (ctrl == nullptr) {
			this->app.getErrorHandler()->alert(__("Motor can't be connected"),
			                                   __("Script: Connection error"),
			                                   wxICON_WARNING);
			return -1;
		} else {
			volatile bool ready = false;
			this->app.getMainFrame()->getDevicePool()->appendDevice(
			    new CalxMotorConstructor(this->app.getMainFrame()->getDevicePool(),
			                             ctrl),
			    const_cast<bool *>(&ready));
			while (!ready) {
			}
			return ctrl->getID();
		}
	}

	device_id_t CalXAppScriptDevices::connectInstrument(
	    DeviceConnectionPrms *prms) {
		SystemManager &sysman = this->app.getSystemManager();
		std::shared_ptr<InstrumentController> ctrl =
		    sysman.getInstrumentControllerSet().connectDevice(prms).lock();
		if (ctrl == nullptr) {
			this->app.getErrorHandler()->alert(__("Instrument can't be connected"),
			                                   __("Script: Connection error"),
			                                   wxICON_WARNING);
			return -1;
		} else {
			volatile bool ready = false;
			this->app.getMainFrame()->getDevicePool()->appendDevice(
			    new CalxInstrumentConstructor(
			        this->app.getMainFrame()->getDevicePool(), ctrl),
			    const_cast<bool *>(&ready));
			while (!ready) {
			}
			return ctrl->getID();
		}
	}

	std::size_t CalXAppScriptDevices::getMotorCount() {
		return this->app.getMainFrame()->getDevicePool()->getMotorCount();
	}

	std::size_t CalXAppScriptDevices::getInstrumentCount() {
		return this->app.getMainFrame()->getDevicePool()->getInstrumentCount();
	}

	std::unique_ptr<CalXScriptMotor> CalXAppScriptDevices::getMotor(
	    device_id_t id) {
		return std::make_unique<CalXAppScriptMotor>(this->app, id);
	}

	std::unique_ptr<CalXScriptInstrument> CalXAppScriptDevices::getInstrument(
	    device_id_t id) {
		return std::make_unique<CalXAppScriptInstrument>(this->app, id);
	}
}  // namespace CalXUI