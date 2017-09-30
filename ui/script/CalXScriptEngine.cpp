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
			wxMessageBox(__("Motor can't be connected"), __("Connection error"),
			             wxICON_WARNING);
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
			wxMessageBox(__("Instrument can't be connected"), __("Connection error"),
			             wxICON_WARNING);
			return false;
		} else {
			this->app.getMainFrame()->getDevicePool()->appendDevice(
			    new CalxInstrumentConstructor(
			        this->app.getMainFrame()->getDevicePool(), ctrl));
			return true;
		}
	}
	
	uint32_t CalXAppScriptEnvironment::getMotorCount() {
		return this->app.getSystemManager()->getMotorCount();
	}
	
	uint32_t CalXAppScriptEnvironment::getInstrumentCount() {
		return this->app.getSystemManager()->getInstrumentCount();
	}
	
	
}  // namespace CalXUI
