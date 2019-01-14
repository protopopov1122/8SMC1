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

#include <iostream>
#include "ui/script/CalXScriptEngine.h"
#include "ui/dev/CalxDeviceConstructor.h"
#include "ui/coord/CalxCoordActions.h"
#include "ui/CalxErrorHandler.h"

namespace CalXUI {

	CalXScriptHookThread::CalXScriptHookThread(std::string path, std::string hook)
	    : wxThread::wxThread(wxTHREAD_DETACHED), path(path), hook(hook) {}

	wxThread::ExitCode CalXScriptHookThread::Entry() {
		std::unique_ptr<CalXScript> scr = wxGetApp().loadScript(this->path);
		scr->call(this->hook);
		return nullptr;
	}

	CalXAppScriptMotor::CalXAppScriptMotor(CalxApp &app, device_id_t motor_id)
	    : app(app), motor_id(motor_id) {}

	bool CalXAppScriptMotor::isValid() {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()
		        ->getMotorControllerSet()
		        .getDeviceController(this->motor_id)
		        .lock();
		return motor != nullptr;
	}

	std::optional<Power> CalXAppScriptMotor::getPower() {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()
		        ->getMotorControllerSet()
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
			ActionResult res = { false, false, ErrorCode::NoError };
			motor->move(pos, speed, false, &res);
			while (!res.ready) {
			}
			if (res.stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.errcode;
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
		        ->getMotorControllerSet()
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
			ActionResult res = { false, false, ErrorCode::NoError };
			motor->roll(tr, &res);
			while (!res.ready) {
			}
			if (res.stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.errcode;
			}
		}
	}

	ErrorCode CalXAppScriptMotor::checkTrailers() {
		std::shared_ptr<MotorController> motor =
		    this->app.getSystemManager()
		        ->getMotorControllerSet()
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
		        ->getMotorControllerSet()
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
		        ->getInstrumentControllerSet()
		        .getDeviceController(this->instrument_id)
		        .lock();
		return instr != nullptr;
	}

	ErrorCode CalXAppScriptInstrument::open_session() {
		std::shared_ptr<InstrumentController> instr =
		    this->app.getSystemManager()
		        ->getInstrumentControllerSet()
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
		        ->getInstrumentControllerSet()
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
		        ->getInstrumentControllerSet()
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
		        ->getInstrumentControllerSet()
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
		        ->getInstrumentControllerSet()
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
		        ->getInstrumentControllerSet()
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
		        ->getInstrumentControllerSet()
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
		        ->getInstrumentControllerSet()
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
		        ->getInstrumentControllerSet()
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
		        ->getInstrumentControllerSet()
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
		SystemManager *sysman = this->app.getSystemManager();
		std::shared_ptr<MotorController> ctrl =
		    sysman->getMotorControllerSet().connectDevice(prms).lock();
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
		SystemManager *sysman = this->app.getSystemManager();
		std::shared_ptr<InstrumentController> ctrl =
		    sysman->getInstrumentControllerSet().connectDevice(prms).lock();
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

	CalXAppScriptPlane::CalXAppScriptPlane(CalxApp &app, std::size_t id)
	    : app(app), plane_id(id) {}

	ErrorCode CalXAppScriptPlane::move(coord_point_t dest, double speed,
	                                   bool sync, bool relative) {
		CalxPlaneHandle *handle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(
		        this->plane_id);
		if (handle == nullptr) {
			return ErrorCode::UnknownResource;
		} else {
			volatile bool ready = false;
			ActionResult res = { false, false, ErrorCode::NoError };
			handle->getController()->move(dest, speed, sync, relative,
			                              const_cast<bool *>(&ready), &res);
			while (!ready) {
			}
			if (res.stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.errcode;
			}
		}
	}

	ErrorCode CalXAppScriptPlane::arc(coord_point_t dest, coord_point_t cen,
	                                  int splitter, double speed, bool clockwise,
	                                  bool relative) {
		CalxPlaneHandle *handle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(
		        this->plane_id);
		if (handle == nullptr) {
			return ErrorCode::UnknownResource;
		} else {
			volatile bool ready = false;
			ActionResult res = { false, false, ErrorCode::NoError };
			handle->getController()->arc(dest, cen, splitter, speed, clockwise,
			                             relative, const_cast<bool *>(&ready), &res);
			while (!ready) {
			}
			if (res.stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.errcode;
			}
		}
	}

	ErrorCode CalXAppScriptPlane::calibrate(TrailerId tid) {
		CalxPlaneHandle *handle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(
		        this->plane_id);
		if (handle == nullptr) {
			return ErrorCode::UnknownResource;
		} else {
			volatile bool ready = false;
			ActionResult res = { false, false, ErrorCode::NoError };
			handle->getController()->calibrate(tid, const_cast<bool *>(&ready), &res);
			while (!ready) {
			}
			if (res.stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.errcode;
			}
		}
	}

	ErrorCode CalXAppScriptPlane::measure(TrailerId tid) {
		CalxPlaneHandle *handle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(
		        this->plane_id);
		if (handle == nullptr) {
			return ErrorCode::UnknownResource;
		} else {
			volatile bool ready = false;
			ActionResult res = { false, false, ErrorCode::NoError };
			handle->getController()->measure(tid, const_cast<bool *>(&ready), &res);
			while (!ready) {
			}
			if (res.stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.errcode;
			}
		}
	}

	ErrorCode CalXAppScriptPlane::move(coord_point_t dest, double speed) {
		CalxPlaneHandle *handle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(
		        this->plane_id);
		if (handle == nullptr) {
			return ErrorCode::UnknownResource;
		} else {
			volatile bool ready = false;
			ActionResult res = { false, false, ErrorCode::NoError };
			handle->getController()->move(dest, speed, const_cast<bool *>(&ready),
			                              &res);
			while (!ready) {
			}
			if (res.stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.errcode;
			}
		}
	}

	ErrorCode CalXAppScriptPlane::configure(coord_point_t dest, double speed) {
		CalxPlaneHandle *handle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(
		        this->plane_id);
		if (handle == nullptr) {
			return ErrorCode::UnknownResource;
		} else {
			volatile bool ready = false;
			ActionResult res = { false, false, ErrorCode::NoError };
			handle->getController()->configure(dest, speed,
			                                   const_cast<bool *>(&ready), &res);
			while (!ready) {
			}
			if (res.stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.errcode;
			}
		}
	}

	std::optional<coord_point_t> CalXAppScriptPlane::getPosition() {
		CalxPlaneHandle *planeHandle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(
		        this->plane_id);
		std::shared_ptr<CoordHandle> handle =
		    planeHandle != nullptr ? planeHandle->getController()->getHandle()
		                           : nullptr;
		coord_point_t pos;
		if (handle == nullptr) {
			return std::optional<coord_point_t>();
		} else {
			pos = handle->getFloatPlane()->getFloatPosition();
			return std::optional<coord_point_t>(pos);
		}
	}

	std::pair<coord_rect_t, ErrorCode> CalXAppScriptPlane::getSize() {
		CalxPlaneHandle *planeHandle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(
		        this->plane_id);
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

	std::optional<bool> CalXAppScriptPlane::isMeasured() {
		CalxPlaneHandle *planeHandle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(
		        this->plane_id);
		std::shared_ptr<CoordHandle> handle =
		    planeHandle != nullptr ? planeHandle->getController()->getHandle()
		                           : nullptr;
		coord_rect_t rect;
		if (handle == nullptr) {
			return std::optional<bool>();
		} else {
			return handle->isMeasured();
		}
	}

	bool CalXAppScriptPlane::positionAsCenter() {
		CalxPlaneHandle *planeHandle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(
		        this->plane_id);
		if (planeHandle != nullptr) {
			motor_point_t offset =
			    planeHandle->getController()->getHandle()->getPosition();
			motor_scale_t scale = planeHandle->getController()->getScale();
			offset.x += planeHandle->getController()->getOffset().x / scale.x;
			offset.y += planeHandle->getController()->getOffset().y / scale.y;
			planeHandle->getController()->setOffset(offset);
			return true;
		} else {
			return false;
		}
	}

	CalXAppScriptPlanes::CalXAppScriptPlanes(CalxApp &app) : app(app) {}

	int32_t CalXAppScriptPlanes::createPlane(device_id_t motor1,
	                                         device_id_t motor2,
	                                         device_id_t instrument) {
		std::shared_ptr<CoordHandle> handle =
		    this->app.getSystemManager()
		        ->getCoordPlaneSet()
		        .createCoord(this->app.getSystemManager()
		                         ->getMotorControllerSet()
		                         .getDeviceController(motor1)
		                         .lock(),
		                     this->app.getSystemManager()
		                         ->getMotorControllerSet()
		                         .getDeviceController(motor2)
		                         .lock(),
		                     this->app.getSystemManager()
		                         ->getInstrumentControllerSet()
		                         .getDeviceController(instrument)
		                         .lock())
		        .lock();
		if (handle == nullptr) {
			return -1;
		} else {
			volatile bool ready = false;
			this->app.getMainFrame()->getPlaneList()->updateList(
			    handle, const_cast<bool *>(&ready));
			while (!ready) {
			}
			return handle->getID();
		}
	}

	std::unique_ptr<CalXScriptPlane> CalXAppScriptPlanes::getPlane(
	    std::size_t id) {
		return std::make_unique<CalXAppScriptPlane>(this->app, id);
	}

	CalXAppScriptUI::CalXAppScriptUI(CalxApp &app)
		: app(app) {}
	
	ErrorCode CalXAppScriptUI::openWatcher(std::size_t id) {
		CalxPlaneHandle *handle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(
		        id);
		if (handle == nullptr) {
			return ErrorCode::UnknownResource;
		} else {
			handle->newWatcher();
			return ErrorCode::NoError;
		}
	}

	CalXAppScriptEnvironment::CalXAppScriptEnvironment(CalxApp &app)
	    : CalXScriptUIEnvironment::CalXScriptUIEnvironment(
	          app.getSystemManager()->getConfiguration(),
	          app.getSystemManager()->getSettingsRepository()),
	      app(app),
	      devices(app),
	      planes(app),
		  ui(app) {}

	CalXScriptDevices &CalXAppScriptEnvironment::getDevices() {
		return this->devices;
	}

	CalXScriptPlanes &CalXAppScriptEnvironment::getPlanes() {
		return this->planes;
	}

	CalXScriptUI &CalXAppScriptEnvironment::getUI() {
		return this->ui;
	}

}  // namespace CalXUI
