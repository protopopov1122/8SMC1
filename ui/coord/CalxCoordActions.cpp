
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

#include "ui/coord/CalxCoordActions.h"
#include "ui/CalxErrorHandler.h"
#include "ctrl-lib/logger/Shortcuts.h"

namespace CalXUI {

	void coordActionFinished(CoordHandle &handle, ErrorCode errcode) {
		Info(wxGetApp().getJournal())
		    << "Coordinate plane #" << handle.getID()
		    << " action finished with code " << static_cast<int>(errcode);
	}

	void coordActionStopped(CoordHandle &handle) {
		Info(wxGetApp().getJournal())
		    << "Coordinate plane #" << handle.getID() << " action stopped";
	}

	CalxCoordActionMove::CalxCoordActionMove(std::shared_ptr<CoordHandle> handle,
	                                         coord_point_t dest, double speed,
	                                         bool sync, bool relative)
	    : handle(handle),
	      dest(dest),
	      speed(speed),
	      sync(sync),
	      relative(relative) {}

	ErrorCode CalxCoordActionMove::perform(SystemManager &sysman) {
		ErrorCode errcode;
		ResourceSession session(*this->handle);
		Info(wxGetApp().getJournal())
		    << "Lineary moving coordinate plane #" << this->handle->getID()
		    << " to " << dest.x << "x" << dest.y
		    << (relative ? "relatively to current position" : "") << " with speed "
		    << speed << " " << (sync ? "synchroniously" : "asynchroniously");
		if (relative) {
			errcode = handle->getFloatPlane()->relativeMove(dest, speed, sync);
		} else {
			errcode = handle->getFloatPlane()->move(dest, speed, sync);
		}
		wxGetApp().getErrorHandler()->handle(errcode);
		coordActionFinished(*handle, errcode);
		return errcode;
	}

	void CalxCoordActionMove::stop() {
		handle->stop();
		coordActionStopped(*handle);
	}

	CalxCoordActionArc::CalxCoordActionArc(std::shared_ptr<CoordHandle> handle,
	                                       coord_point_t dest, coord_point_t cen,
	                                       int splitter, double speed,
	                                       bool clockwise, bool relative)
	    : handle(handle),
	      dest(dest),
	      cen(cen),
	      splitter(splitter),
	      speed(speed),
	      clockwise(clockwise),
	      relative(relative) {}

	ErrorCode CalxCoordActionArc::perform(SystemManager &sysman) {
		ErrorCode errcode;
		ResourceSession session(*this->handle);
		Info(wxGetApp().getJournal())
		    << "Arc " << (clockwise ? "clockwise" : "counter-clockwise")
		    << " moving coordinate plane #" << this->handle->getID() << " to "
		    << dest.x << "x" << dest.y << " with center in " << cen.x << "x"
		    << cen.y << " splitted into " << splitter << " segments"
		    << " with speed " << speed;
		if (relative) {
			errcode = handle->getFloatPlane()->relativeArc(dest, cen, splitter, speed,
			                                               clockwise);
		} else {
			errcode =
			    handle->getFloatPlane()->arc(dest, cen, splitter, speed, clockwise);
		}
		wxGetApp().getErrorHandler()->handle(errcode);
		coordActionFinished(*handle, errcode);
		return errcode;
	}

	void CalxCoordActionArc::stop() {
		handle->stop();
		coordActionStopped(*handle);
	}

	CalxCoordActionCalibrate::CalxCoordActionCalibrate(
	    std::shared_ptr<CoordHandle> handle, TrailerId tr)
	    : handle(handle), trailer(tr) {}

	ErrorCode CalxCoordActionCalibrate::perform(SystemManager &sysman) {
		ResourceSession session(*this->handle);
		Info(wxGetApp().getJournal())
		    << "Calibrating coordinate plane #" << handle->getID()
		    << " to the trailer #" << static_cast<int>(trailer);
		ErrorCode errcode = handle->calibrate(trailer);
		wxGetApp().getErrorHandler()->handle(errcode);
		coordActionFinished(*handle, errcode);
		return errcode;
	}

	void CalxCoordActionCalibrate::stop() {
		this->handle->stop();
		coordActionStopped(*handle);
	}

	CalxCoordActionMeasure::CalxCoordActionMeasure(
	    std::shared_ptr<CoordHandle> handle, TrailerId tr)
	    : handle(handle), trailer(tr) {}

	ErrorCode CalxCoordActionMeasure::perform(SystemManager &sysman) {
		ResourceSession session(*this->handle);
		Info(wxGetApp().getJournal())
		    << "Measuring coordinate plane #" << handle->getID()
		    << " to the trailer #" << static_cast<int>(trailer);
		ErrorCode errcode = handle->measure(trailer);
		wxGetApp().getErrorHandler()->handle(errcode);
		coordActionFinished(*handle, errcode);
		return errcode;
	}

	void CalxCoordActionMeasure::stop() {
		this->handle->stop();
		coordActionStopped(*handle);
	}

	CalxCoordActionConfigure::CalxCoordActionConfigure(
	    std::shared_ptr<CoordHandle> handle, CalxCoordController *ctrl,
	    coord_point_t dest, double speed)
	    : handle(handle),
	      controller(ctrl),
	      dest(dest),
	      speed(speed),
	      work(false) {}

	ErrorCode CalxCoordActionConfigure::perform(SystemManager &sysman) {
		ResourceSession session(*this->handle);
		Info(wxGetApp().getJournal())
		    << "Configuring coordinate plane #" << handle->getID()
		    << " with center " << dest.x << "x" << dest.y << " and movement speed "
		    << speed;
		work = true;
		motor_point_t offset = { 0, 0 };
		controller->setOffset(offset);
		ErrorCode errcode = this->handle->measure(TrailerId::Trailer1);
		if (errcode != ErrorCode::NoError) {
			work = false;
		}
		coord_rect_t size = this->handle->getFloatPlane()->getFloatSize();
		double x = static_cast<double>((((double) size.w) * this->dest.x) + size.x);
		double y = static_cast<double>((((double) size.h) * this->dest.y) + size.y);
		coord_point_t dest = { x, y };
		if (work) {
			errcode = handle->getFloatPlane()->move(dest, speed, false);
		}
		if (work && errcode == ErrorCode::NoError) {
			controller->setOffset(handle->getPosition());
		}
		wxGetApp().getErrorHandler()->handle(errcode);
		coordActionFinished(*handle, errcode);
		return errcode;
	}

	void CalxCoordActionConfigure::stop() {
		this->work = false;
		handle->stop();
		coordActionStopped(*handle);
	}

	CalxCoordActionGraphBuild::CalxCoordActionGraphBuild(
	    std::shared_ptr<CoordHandle> handle,
	    std::shared_ptr<CoordTranslator> trans,
	    std::unique_ptr<GraphBuilder> builder, double speed)
	    : handle(handle),
	      translator(trans),
	      builder(std::move(builder)),
	      speed(speed) {
		this->state = std::make_shared<TaskState>();
		this->state->plane = nullptr;
		this->state->work = false;
	}

	ErrorCode CalxCoordActionGraphBuild::perform(SystemManager &sysman) {
		ResourceSession session(*this->handle);
		Info(wxGetApp().getJournal())
		    << "Plotting graph on coordinate plane #" << handle->getID()
		    << " function " << *builder << " with speed " << speed;
		ErrorCode errcode = builder->floatBuild(sysman, handle->getFloatPlane(),
		                                        *translator, speed, *state);
		wxGetApp().getErrorHandler()->handle(errcode);
		coordActionFinished(*handle, errcode);
		return errcode;
	}

	void CalxCoordActionGraphBuild::stop() {
		this->state->stop();
		coordActionStopped(*handle);
	}

	CalxCoordActionGraphPreview::CalxCoordActionGraphPreview(
	    CalxVirtualPlaneDialog *dialog, std::shared_ptr<CoordTranslator> trans,
	    std::unique_ptr<GraphBuilder> builder, double speed)
	    : dialog(dialog),
	      translator(trans),
	      builder(std::move(builder)),
	      speed(speed) {
		this->state = std::make_shared<TaskState>();
		this->state->plane = nullptr;
		this->state->work = false;
	}

	ErrorCode CalxCoordActionGraphPreview::perform(SystemManager &sysman) {
		dialog->Enable(false);
		ErrorCode errcode = builder->floatBuild(sysman, dialog->getFloatPlane(),
		                                        *translator, speed, *state);
		wxGetApp().getErrorHandler()->handle(errcode);
		dialog->Refresh();
		dialog->Enable(true);
		return errcode;
	}

	void CalxCoordActionGraphPreview::stop() {
		this->state->stop();
	}
}  // namespace CalXUI
