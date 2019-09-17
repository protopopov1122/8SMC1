/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calx/ui/script/CalXScriptEngine.h"
#include "calx/ui/coord/CalxCoordActions.h"
#include "calx/ui/CalxErrorHandler.h"

namespace CalX::UI {

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
			auto res = handle->getController()->move(dest, speed, sync, relative);
			res.wait();
			if (res.getStatus() == CalxActionStatus::Stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.getError();
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
			auto res = handle->getController()->arc(dest, cen, splitter, speed,
			                                        clockwise, relative);
			res.wait();
			if (res.getStatus() == CalxActionStatus::Stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.getError();
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
			auto res = handle->getController()->calibrate(tid);
			res.wait();
			if (res.getStatus() == CalxActionStatus::Stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.getError();
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
			auto res = handle->getController()->measure(tid);
			res.wait();
			if (res.getStatus() == CalxActionStatus::Stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.getError();
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
			auto res = handle->getController()->move(dest, speed);
			res.wait();
			if (res.getStatus() == CalxActionStatus::Stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.getError();
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
			auto res = handle->getController()->configure(dest, speed);
			res.wait();
			if (res.getStatus() == CalxActionStatus::Stopped) {
				return ErrorCode::Interrupted;
			} else {
				return res.getError();
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
		        .getCoordPlaneSet()
		        .createCoord(this->app.getSystemManager()
		                         .getMotorControllerSet()
		                         .getDeviceController(motor1)
		                         .lock(),
		                     this->app.getSystemManager()
		                         .getMotorControllerSet()
		                         .getDeviceController(motor2)
		                         .lock(),
		                     this->app.getSystemManager()
		                         .getInstrumentControllerSet()
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

	std::size_t CalXAppScriptPlanes::getPlaneCount() {
		return this->app.getMainFrame()->getPlaneList()->getPlaneCount();
	}

	std::unique_ptr<CalXScriptPlane> CalXAppScriptPlanes::getPlane(
	    std::size_t id) {
		return std::make_unique<CalXAppScriptPlane>(this->app, id);
	}
}  // namespace CalX::UI