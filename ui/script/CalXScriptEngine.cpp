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
#include "ui/coord/CalxCoordActions.h"

namespace CalXUI {

	CalXScriptHookThread::CalXScriptHookThread(std::string path, std::string hook)
	    : wxThread::wxThread(wxTHREAD_DETACHED), path(path), hook(hook) {}

	wxThread::ExitCode CalXScriptHookThread::Entry() {
		std::unique_ptr<CalXScript> scr = wxGetApp().loadScript(this->path);
		scr->call("init");
		return nullptr;
	}

	CalXAppScriptEnvironment::CalXAppScriptEnvironment(CalxApp &app)
	    : CalXScriptEnvironment::CalXScriptEnvironment(
	          app.getSystemManager()->getConfiguration()),
	      app(app) {}

	device_id_t CalXAppScriptEnvironment::connectSerialMotor(uint8_t port,
	                                                         uint32_t baudrate,
	                                                         uint8_t parity) {
		SystemManager *sysman = this->app.getSystemManager();
		DeviceSerialPortConnectionPrms prms;
		prms.port = port;
		prms.speed = baudrate;
		prms.parity = static_cast<SerialPortParity>(parity);
		std::shared_ptr<MotorController> ctrl = sysman->connectMotor(&prms).lock();
		if (ctrl == nullptr) {
			wxMessageBox(__("Motor can't be connected"),
			             __("Script: Connection error"), wxICON_WARNING);
			return -1;
		} else {
			bool ready = false;
			this->app.getMainFrame()->getDevicePool()->appendDevice(
			    new CalxMotorConstructor(this->app.getMainFrame()->getDevicePool(),
			                             ctrl),
			    &ready);
			while (!ready) {
			}
			return ctrl->getID();
		}
	}

	device_id_t CalXAppScriptEnvironment::connectSerialInstrument(
	    uint8_t port, uint32_t baudrate, uint8_t parity) {
		SystemManager *sysman = this->app.getSystemManager();
		DeviceSerialPortConnectionPrms prms;
		prms.port = port;
		prms.speed = baudrate;
		prms.parity = static_cast<SerialPortParity>(parity);
		std::shared_ptr<InstrumentController> ctrl =
		    sysman->connectInstrument(&prms).lock();
		if (ctrl == nullptr) {
			wxMessageBox(__("Instrument can't be connected"),
			             __("Script: Connection error"), wxICON_WARNING);
			return -1;
		} else {
			bool ready = false;
			this->app.getMainFrame()->getDevicePool()->appendDevice(
			    new CalxInstrumentConstructor(
			        this->app.getMainFrame()->getDevicePool(), ctrl),
			    &ready);
			while (!ready) {
			}
			return ctrl->getID();
		}
	}

	size_t CalXAppScriptEnvironment::getMotorCount() {
		return this->app.getMainFrame()->getDevicePool()->getMotorCount();
	}

	size_t CalXAppScriptEnvironment::getInstrumentCount() {
		return this->app.getMainFrame()->getDevicePool()->getInstrumentCount();
	}

	std::pair<Power, ErrorCode> CalXAppScriptEnvironment::getMotorPower(
	    device_id_t id) {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()->getMotorController(id).lock();
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
		CalxMotorHandle *motor =
		    this->app.getMainFrame()->getDevicePool()->getMotor(id);
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			return motor->setPower(power);
		}
	}

	ErrorCode CalXAppScriptEnvironment::motorMove(device_id_t id,
	                                              motor_coord_t pos,
	                                              float speed) {
		CalxMotorHandle *motor =
		    this->app.getMainFrame()->getDevicePool()->getMotor(id);
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			ActionResult res = { false, false, ErrorCode::NoError };
			motor->move(pos, speed, false, &res);
			while (!res.ready) {
			}
			if (res.stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.errcode;
				;
			}
		}
	}

	ErrorCode CalXAppScriptEnvironment::motorRelativeMove(device_id_t id,
	                                                      motor_coord_t pos,
	                                                      float speed) {
		CalxMotorHandle *motor =
		    this->app.getMainFrame()->getDevicePool()->getMotor(id);
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			ActionResult res = { false, false, ErrorCode::NoError };
			motor->move(pos, speed, true, &res);
			while (!res.ready) {
			}
			if (res.stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.errcode;
				;
			}
		}
	}

	ErrorCode CalXAppScriptEnvironment::motorStop(device_id_t id) {
		CalxMotorHandle *motor =
		    this->app.getMainFrame()->getDevicePool()->getMotor(id);
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			motor->stopMovement();
			return ErrorCode::NoError;
		}
	}

	std::pair<motor_coord_t, ErrorCode>
	    CalXAppScriptEnvironment::getMotorPosition(device_id_t id) {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()->getMotorController(id).lock();
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
		CalxMotorHandle *motor =
		    this->app.getMainFrame()->getDevicePool()->getMotor(id);
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			ActionResult res = { false, false, ErrorCode::NoError };
			motor->roll(tr, &res);
			while (!res.ready) {
			}
			if (res.stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.errcode;
				;
			}
		}
	}

	std::pair<bool, ErrorCode> CalXAppScriptEnvironment::motorCheckTrailers(
	    device_id_t id) {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()->getMotorController(id).lock();
		if (motor == nullptr) {
			wxMessageBox(FORMAT(__("Motor %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown motor"), wxICON_WARNING);
			return std::make_pair(false, ErrorCode::UnknownResource);
		} else {
			return std::make_pair(motor->checkTrailers() != ErrorCode::NoError,
			                      ErrorCode::NoError);
		}
	}

	ErrorCode CalXAppScriptEnvironment::motorWaitWhileRunning(device_id_t id) {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()->getMotorController(id).lock();
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
		    this->app.getSystemManager()->getInstrumentController(id).lock();
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
		    this->app.getSystemManager()->getInstrumentController(id).lock();
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			return instr->close_session();
		}
	}

	ErrorCode CalXAppScriptEnvironment::instrumentEnable(device_id_t id,
	                                                     bool en) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id).lock();
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			return instr->enable(en);
		}
	}

	std::pair<bool, ErrorCode> CalXAppScriptEnvironment::instrumentIsEnabled(
	    device_id_t id) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id).lock();
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return std::make_pair(false, ErrorCode::UnknownResource);
		} else {
			return std::make_pair(instr->isEnabled(), ErrorCode::NoError);
		}
	}

	ErrorCode CalXAppScriptEnvironment::instrumentSetRunnable(device_id_t id,
	                                                          bool en) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id).lock();
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return ErrorCode::UnknownResource;
		} else {
			instr->setRunnable(en);
			return ErrorCode::NoError;
		}
	}

	std::pair<bool, ErrorCode> CalXAppScriptEnvironment::instrumentIsRunnable(
	    device_id_t id) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id).lock();
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return std::make_pair(false, ErrorCode::UnknownResource);
		} else {
			return std::make_pair(instr->isRunnable(), ErrorCode::NoError);
		}
	}

	std::pair<InstrumentMode, ErrorCode>
	    CalXAppScriptEnvironment::instrumentGetMode(device_id_t id) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id).lock();
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return std::make_pair(InstrumentMode::Off, ErrorCode::UnknownResource);
		} else {
			return std::make_pair(instr->getMode(), ErrorCode::NoError);
		}
	}

	std::pair<bool, ErrorCode> CalXAppScriptEnvironment::instrumentSetMode(
	    device_id_t id, InstrumentMode mode) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id).lock();
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return std::make_pair(false, ErrorCode::UnknownResource);
		} else {
			return std::make_pair(instr->setMode(mode), ErrorCode::NoError);
		}
	}

	std::pair<bool, ErrorCode>
	    CalXAppScriptEnvironment::instrumentIsSessionOpened(device_id_t id) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id).lock();
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return std::make_pair(false, ErrorCode::UnknownResource);
		} else {
			return std::make_pair(instr->isSessionOpened(), ErrorCode::NoError);
		}
	}

	std::pair<std::string, ErrorCode> CalXAppScriptEnvironment::instrumentGetInfo(
	    device_id_t id) {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()->getInstrumentController(id).lock();
		if (instr == nullptr) {
			wxMessageBox(FORMAT(__("Instrument %" DEVICE_ID_FMT " not found!"), id),
			             __("Script: Unknown instrument"), wxICON_WARNING);
			return std::make_pair("", ErrorCode::UnknownResource);
		} else {
			return std::make_pair(instr->getInfo(), ErrorCode::NoError);
		}
	}

	int32_t CalXAppScriptEnvironment::createCoordPlane(device_id_t m1,
	                                                   device_id_t m2,
	                                                   device_id_t instr) {
		std::shared_ptr<CoordHandle> handle =
		    this->app.getSystemManager()->createCoord(m1, m2, instr).lock();
		if (handle == nullptr) {
			return -1;
		} else {
			bool ready = false;
			this->app.getMainFrame()->getPlaneList()->updateList(handle, &ready);
			while (!ready) {
			}
			return handle->getID();
		}
	}

	ErrorCode CalXAppScriptEnvironment::planeMove(size_t id, coord_point_t dest,
	                                              double speed, bool sync,
	                                              bool relative) {
		CalxPlaneHandle *handle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(id);
		if (handle == nullptr) {
			return ErrorCode::UnknownResource;
		} else {
			bool ready = false;
			ActionResult res = { false, false, ErrorCode::NoError };
			handle->getController()->move(dest, speed, sync, relative, &ready, &res);
			while (!ready) {
			}
			if (res.stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.errcode;
			}
		}
	}

	ErrorCode CalXAppScriptEnvironment::planeArc(size_t id, coord_point_t dest,
	                                             coord_point_t cen, int splitter,
	                                             double speed, bool clockwise,
	                                             bool relative) {
		CalxPlaneHandle *handle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(id);
		if (handle == nullptr) {
			return ErrorCode::UnknownResource;
		} else {
			bool ready = false;
			ActionResult res = { false, false, ErrorCode::NoError };
			handle->getController()->arc(dest, cen, splitter, speed, clockwise,
			                             relative, &ready, &res);
			while (!ready) {
			}
			if (res.stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.errcode;
			}
		}
	}

	ErrorCode CalXAppScriptEnvironment::planeCalibrate(size_t id, TrailerId tid) {
		CalxPlaneHandle *handle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(id);
		if (handle == nullptr) {
			return ErrorCode::UnknownResource;
		} else {
			bool ready = false;
			ActionResult res = { false, false, ErrorCode::NoError };
			handle->getController()->calibrate(tid, &ready, &res);
			while (!ready) {
			}
			if (res.stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.errcode;
			}
		}
	}

	ErrorCode CalXAppScriptEnvironment::planeMeasure(size_t id, TrailerId tid) {
		CalxPlaneHandle *handle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(id);
		if (handle == nullptr) {
			return ErrorCode::UnknownResource;
		} else {
			bool ready = false;
			ActionResult res = { false, false, ErrorCode::NoError };
			handle->getController()->measure(tid, &ready, &res);
			while (!ready) {
			}
			if (res.stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.errcode;
			}
		}
	}

	ErrorCode CalXAppScriptEnvironment::planeMove(size_t id, coord_point_t dest,
	                                              double speed) {
		CalxPlaneHandle *handle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(id);
		if (handle == nullptr) {
			return ErrorCode::UnknownResource;
		} else {
			bool ready = false;
			ActionResult res = { false, false, ErrorCode::NoError };
			handle->getController()->move(dest, speed, &ready, &res);
			while (!ready) {
			}
			if (res.stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.errcode;
			}
		}
	}

	ErrorCode CalXAppScriptEnvironment::planeConfigure(size_t id,
	                                                   coord_point_t dest,
	                                                   double speed) {
		CalxPlaneHandle *handle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(id);
		if (handle == nullptr) {
			return ErrorCode::UnknownResource;
		} else {
			bool ready = false;
			ActionResult res = { false, false, ErrorCode::NoError };
			handle->getController()->configure(dest, speed, &ready, &res);
			while (!ready) {
			}
			if (res.stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.errcode;
			}
		}
	}

	ErrorCode CalXAppScriptEnvironment::planeNewWatcher(size_t id) {
		CalxPlaneHandle *handle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(id);
		if (handle == nullptr) {
			return ErrorCode::UnknownResource;
		} else {
			handle->newWatcher();
			return ErrorCode::NoError;
		}
	}

	std::pair<coord_point_t, ErrorCode>
	    CalXAppScriptEnvironment::planeGetPosition(size_t id) {
		CalxPlaneHandle *planeHandle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(id);
		std::shared_ptr<CoordHandle> handle =
		    planeHandle != nullptr ? planeHandle->getController()->getHandle()
		                           : nullptr;
		coord_point_t pos;
		if (handle == nullptr) {
			return std::make_pair(pos, ErrorCode::UnknownResource);
		} else {
			pos = handle->getFloatPlane()->getFloatPosition();
			return std::make_pair(pos, ErrorCode::NoError);
		}
	}

	std::pair<coord_rect_t, ErrorCode> CalXAppScriptEnvironment::planeGetSize(
	    size_t id) {
		CalxPlaneHandle *planeHandle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(id);
		std::shared_ptr<CoordHandle> handle =
		    planeHandle != nullptr ? planeHandle->getController()->getHandle()
		                           : nullptr;
		coord_rect_t rect;
		if (handle == nullptr) {
			return std::make_pair(rect, ErrorCode::UnknownResource);
		} else if (!handle->isMeasured()) {
			return std::make_pair(rect, ErrorCode::OperationUnavailable);
		} else {
			coord_rect_t rect = handle->getFloatPlane()->getFloatSize();
			return std::make_pair(rect, ErrorCode::NoError);
		}
	}

	std::pair<bool, ErrorCode> CalXAppScriptEnvironment::planeIsMeasured(
	    size_t id) {
		CalxPlaneHandle *planeHandle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(id);
		std::shared_ptr<CoordHandle> handle =
		    planeHandle != nullptr ? planeHandle->getController()->getHandle()
		                           : nullptr;
		coord_rect_t rect;
		if (handle == nullptr) {
			return std::make_pair(false, ErrorCode::UnknownResource);
		} else {
			return std::make_pair(handle->isMeasured(), ErrorCode::NoError);
		}
	}

}  // namespace CalXUI
