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

#include <iostream>
#include "ui/script/CalXScriptEngine.h"
#include "ui/dev/CalxDeviceConstructor.h"

namespace CalXUI {

	CalXAppScriptEnvironment::CalXAppScriptEnvironment(CalxApp &app)
	    : CalXScriptEnvironment::CalXScriptEnvironment(
	          app.getSystemManager()->getConfiguration()),
	      app(app) {}

	bool CalXAppScriptEnvironment::connectSerialMotor(uint8_t port,
	                                                  uint32_t baudrate,
	                                                  uint8_t parity) {
		SystemManager *sysman = this->app.getSystemManager();
		DeviceSerialPortConnectionPrms prms;
		prms.port = port;
		prms.speed = baudrate;
		prms.parity = static_cast<SerialPortParity>(parity);
		std::shared_ptr<MotorController> ctrl = sysman->connectMotor(&prms);
		if (ctrl == nullptr) {
			wxMessageBox(__("Motor can't be connected"),
			             __("Script: Connection error"), wxICON_WARNING);
			return false;
		} else {
			this->app.getMainFrame()->getDevicePool()->appendDevice(
			    new CalxMotorConstructor(this->app.getMainFrame()->getDevicePool(),
			                             ctrl));
			return true;
		}
	}

	bool CalXAppScriptEnvironment::connectSerialInstrument(uint8_t port,
	                                                       uint32_t baudrate,
	                                                       uint8_t parity) {
		SystemManager *sysman = this->app.getSystemManager();
		DeviceSerialPortConnectionPrms prms;
		prms.port = port;
		prms.speed = baudrate;
		prms.parity = static_cast<SerialPortParity>(parity);
		std::shared_ptr<InstrumentController> ctrl =
		    sysman->connectInstrument(&prms);
		if (ctrl == nullptr) {
			wxMessageBox(__("Instrument can't be connected"),
			             __("Script: Connection error"), wxICON_WARNING);
			return false;
		} else {
			this->app.getMainFrame()->getDevicePool()->appendDevice(
			    new CalxInstrumentConstructor(
			        this->app.getMainFrame()->getDevicePool(), ctrl));
			return true;
		}
	}

	size_t CalXAppScriptEnvironment::getMotorCount() {
		return this->app.getSystemManager()->getMotorCount();
	}

	size_t CalXAppScriptEnvironment::getInstrumentCount() {
		return this->app.getSystemManager()->getInstrumentCount();
	}

	std::pair<Power, ErrorCode> CalXAppScriptEnvironment::getMotorPower(device_id_t id) {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()->getMotorController(id);
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return std::make_pair(Power::NoPower, ErrorCode::UnknownResource);
		} else {
			return std::make_pair(motor->getPowerState(), ErrorCode::NoError);
		}
	}

	ErrorCode CalXAppScriptEnvironment::enableMotorPower(device_id_t id,
	                                                     bool power) {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()->getMotorController(id);
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			return motor->enablePower(power);
		}
	}

	ErrorCode CalXAppScriptEnvironment::motorMove(device_id_t id,
	                                              motor_coord_t pos,
	                                              float speed) {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()->getMotorController(id);
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			return motor->startMove(pos, speed);
		}
	}

	ErrorCode CalXAppScriptEnvironment::motorRelativeMove(device_id_t id,
	                                                      motor_coord_t pos,
	                                                      float speed) {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()->getMotorController(id);
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			return motor->startRelativeMove(pos, speed);
		}
	}

	ErrorCode CalXAppScriptEnvironment::motorStop(device_id_t id) {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()->getMotorController(id);
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			motor->stop();
			return ErrorCode::NoError;
		}
	}

	std::pair<motor_coord_t, ErrorCode> CalXAppScriptEnvironment::getMotorPosition(device_id_t id) {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()->getMotorController(id);
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return std::make_pair(0, ErrorCode::UnknownResource);
		} else {
			return std::make_pair(motor->getPosition(), ErrorCode::NoError);
		}
	}

	ErrorCode CalXAppScriptEnvironment::motorMoveToTrailer(device_id_t id,
	                                                       TrailerId tr) {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()->getMotorController(id);
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			return motor->moveToTrailer(static_cast<int>(tr));
		}
	}

	std::pair<bool, ErrorCode> CalXAppScriptEnvironment::motorCheckTrailers(device_id_t id) {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()->getMotorController(id);
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return std::make_pair(false, ErrorCode::UnknownResource);
		} else {
			return std::make_pair(motor->checkTrailers(), ErrorCode::NoError);
		}
	}

	ErrorCode CalXAppScriptEnvironment::motorWaitWhileRunning(device_id_t id) {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()->getMotorController(id);
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			return motor->waitWhileRunning();
		}
	}
	
	ErrorCode CalXAppScriptEnvironment::instrumentOpenSession(device_id_t id) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id);
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			return instr->open_session();
		}
	}
	
	ErrorCode CalXAppScriptEnvironment::instrumentCloseSession(device_id_t id) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id);
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			return instr->close_session();
		}
	}
	
	ErrorCode CalXAppScriptEnvironment::instrumentEnable(device_id_t id, bool en) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id);
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			return instr->enable(en);
		}
	}
	
	std::pair<bool, ErrorCode> CalXAppScriptEnvironment::instrumentIsEnabled(device_id_t id) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id);
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return std::make_pair(false, ErrorCode::UnknownResource);
		} else {
			return std::make_pair(instr->isEnabled(), ErrorCode::NoError);
		}
	}
	
	ErrorCode CalXAppScriptEnvironment::instrumentSetRunnable(device_id_t id, bool en) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id);
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			instr->setRunnable(en);
			return ErrorCode::NoError;
		}
	}
	
	std::pair<bool, ErrorCode> CalXAppScriptEnvironment::instrumentIsRunnable(device_id_t id) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id);
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return std::make_pair(false, ErrorCode::UnknownResource);
		} else {
			return std::make_pair(instr->isRunnable(), ErrorCode::NoError);
		}
	}
	
	std::pair<InstrumentMode, ErrorCode> CalXAppScriptEnvironment::instrumentGetMode(device_id_t id) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id);
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return std::make_pair(InstrumentMode::Off, ErrorCode::UnknownResource);
		} else {
			return std::make_pair(instr->getMode(), ErrorCode::NoError);
		}
	}
	
	std::pair<bool, ErrorCode> CalXAppScriptEnvironment::instrumentSetMode(device_id_t id, InstrumentMode mode) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id);
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return std::make_pair(false, ErrorCode::UnknownResource);
		} else {
			return std::make_pair(instr->setMode(mode), ErrorCode::NoError);
		}
	}
	
	std::pair<bool, ErrorCode> CalXAppScriptEnvironment::instrumentIsSessionOpened(device_id_t id) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id);
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return std::make_pair(false, ErrorCode::UnknownResource);
		} else {
			return std::make_pair(instr->isSessionOpened(), ErrorCode::NoError);
		}
	}
	
	std::pair<std::string, ErrorCode> CalXAppScriptEnvironment::instrumentGetInfo(device_id_t id) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id);
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return std::make_pair("", ErrorCode::UnknownResource);
		} else {
			return std::make_pair(instr->getInfo(), ErrorCode::NoError);
		}
	}

}  // namespace CalXUI
