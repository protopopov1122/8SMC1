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

	Power CalXAppScriptEnvironment::getMotorPower(device_id_t id) {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()->getMotorController(id);
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return Power::NoPower;
		} else {
			return motor->getPowerState();
		}
	}

	ErrorCode CalXAppScriptEnvironment::enableMotorPower(device_id_t id,
	                                                     bool power) {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()->getMotorController(id);
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::LowLevelError;
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
			return ErrorCode::LowLevelError;
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
			return ErrorCode::LowLevelError;
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
		} else {
			motor->stop();
		}
		return ErrorCode::NoError;
	}

	motor_coord_t CalXAppScriptEnvironment::getMotorPosition(device_id_t id) {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()->getMotorController(id);
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return 0;
		} else {
			return motor->getPosition();
		}
	}

	ErrorCode CalXAppScriptEnvironment::motorMoveToTrailer(device_id_t id,
	                                                       TrailerId tr) {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()->getMotorController(id);
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::NoError;
		} else {
			return motor->moveToTrailer(static_cast<int>(tr));
		}
	}

	bool CalXAppScriptEnvironment::motorCheckTrailers(device_id_t id) {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()->getMotorController(id);
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return false;
		} else {
			return motor->checkTrailers();
		}
	}

	ErrorCode CalXAppScriptEnvironment::motorWaitWhileRunning(device_id_t id) {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()->getMotorController(id);
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::NoError;
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
			return ErrorCode::NoError;
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
			return ErrorCode::NoError;
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
			return ErrorCode::NoError;
		} else {
			return instr->enable(en);
		}
	}
	
	bool CalXAppScriptEnvironment::instrumentIsEnabled(device_id_t id) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id);
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return false;
		} else {
			return instr->isEnabled();
		}
	}
	
	void CalXAppScriptEnvironment::instrumentSetRunnable(device_id_t id, bool en) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id);
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
		} else {
			instr->setRunnable(en);
		}
	}
	
	bool CalXAppScriptEnvironment::instrumentIsRunnable(device_id_t id) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id);
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return false;
		} else {
			return instr->isRunnable();
		}
	}
	
	InstrumentMode CalXAppScriptEnvironment::instrumentGetMode(device_id_t id) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id);
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return InstrumentMode::Off;
		} else {
			return instr->getMode();
		}
	}
	
	bool CalXAppScriptEnvironment::instrumentSetMode(device_id_t id, InstrumentMode mode) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id);
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return false;
		} else {
			return instr->setMode(mode);
		}
	}
	
	bool CalXAppScriptEnvironment::instrumentIsSessionOpened(device_id_t id) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id);
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return false;
		} else {
			return instr->isSessionOpened();
		}
	}
	
	std::string CalXAppScriptEnvironment::instrumentGetInfo(device_id_t id) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id);
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return "";
		} else {
			return instr->getInfo();
		}
	}

}  // namespace CalXUI
