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

#include "calx/ctrl-lib/actions/PlaneActions.h"
#include "calx/ctrl-lib/SystemManager.h"
#include "calx/ctrl-lib/logger/Shortcuts.h"

namespace CalX {

	void coordActionFinished(CoordHandle &handle, ErrorCode errcode,
	                         JournalLogger &journal) {
		Info(journal) << "Coordinate plane #" << handle.getID()
		              << " action finished with code " << static_cast<int>(errcode);
	}

	void coordActionStopped(CoordHandle &handle, JournalLogger &journal) {
		Info(journal) << "Coordinate plane #" << handle.getID()
		              << " action stopped";
	}

	CalxCoordBaseAction::CalxCoordBaseAction(std::shared_ptr<CoordHandle> handle,
	                                         ErrorHandlerCallback error_handler,
	                                         JournalLogger &journal)
	    : handle(handle), error_handler(error_handler), journal(journal) {}

	CalxCoordActionMove::CalxCoordActionMove(std::shared_ptr<CoordHandle> handle,
	                                         ErrorHandlerCallback error_handler,
	                                         JournalLogger &journal,
	                                         coord_point_t dest, double speed,
	                                         bool sync, bool relative)
	    : CalxCoordBaseAction::CalxCoordBaseAction(handle, error_handler,
	                                               journal),
	      dest(dest),
	      speed(speed),
	      sync(sync),
	      relative(relative) {}

	ErrorCode CalxCoordActionMove::perform(SystemManager &sysman) {
		ErrorCode errcode;
		ResourceSession session(*this->handle);
		Info(this->journal) << "Lineary moving coordinate plane #"
		                    << this->handle->getID() << " to " << dest.x << "x"
		                    << dest.y
		                    << (relative ? "relatively to current position" : "")
		                    << " with speed " << speed << " "
		                    << (sync ? "synchroniously" : "asynchroniously");
		if (relative) {
			errcode = handle->getFloatPlane()->relativeMove(dest, speed, sync);
		} else {
			errcode = handle->getFloatPlane()->move(dest, speed, sync);
		}
		this->error_handler(errcode);
		coordActionFinished(*handle, errcode, this->journal);
		return errcode;
	}

	void CalxCoordActionMove::stop() {
		handle->stop();
		coordActionStopped(*handle, this->journal);
	}

	CalxCoordActionArc::CalxCoordActionArc(std::shared_ptr<CoordHandle> handle,
	                                       ErrorHandlerCallback error_handler,
	                                       JournalLogger &journal,
	                                       coord_point_t dest, coord_point_t cen,
	                                       int splitter, double speed,
	                                       bool clockwise, bool relative)
	    : CalxCoordBaseAction::CalxCoordBaseAction(handle, error_handler,
	                                               journal),
	      dest(dest),
	      cen(cen),
	      splitter(splitter),
	      speed(speed),
	      clockwise(clockwise),
	      relative(relative) {}

	ErrorCode CalxCoordActionArc::perform(SystemManager &sysman) {
		ErrorCode errcode;
		ResourceSession session(*this->handle);
		Info(this->journal) << "Arc "
		                    << (clockwise ? "clockwise" : "counter-clockwise")
		                    << " moving coordinate plane #" << this->handle->getID()
		                    << " to " << dest.x << "x" << dest.y
		                    << " with center in " << cen.x << "x" << cen.y
		                    << " splitted into " << splitter << " segments"
		                    << " with speed " << speed;
		if (relative) {
			errcode = handle->getFloatPlane()->relativeArc(dest, cen, splitter, speed,
			                                               clockwise);
		} else {
			errcode =
			    handle->getFloatPlane()->arc(dest, cen, splitter, speed, clockwise);
		}
		this->error_handler(errcode);
		coordActionFinished(*handle, errcode, this->journal);
		return errcode;
	}

	void CalxCoordActionArc::stop() {
		handle->stop();
		coordActionStopped(*handle, this->journal);
	}

	CalxCoordActionCalibrate::CalxCoordActionCalibrate(
	    std::shared_ptr<CoordHandle> handle, ErrorHandlerCallback error_handler,
	    JournalLogger &journal, TrailerId tr)
	    : CalxCoordBaseAction::CalxCoordBaseAction(handle, error_handler,
	                                               journal),
	      trailer(tr) {}

	ErrorCode CalxCoordActionCalibrate::perform(SystemManager &sysman) {
		ResourceSession session(*this->handle);
		Info(this->journal) << "Calibrating coordinate plane #" << handle->getID()
		                    << " to the trailer #" << static_cast<int>(trailer);
		ErrorCode errcode = handle->calibrate(trailer);
		this->error_handler(errcode);
		coordActionFinished(*handle, errcode, this->journal);
		return errcode;
	}

	void CalxCoordActionCalibrate::stop() {
		this->handle->stop();
		coordActionStopped(*handle, this->journal);
	}

	CalxCoordActionMeasure::CalxCoordActionMeasure(
	    std::shared_ptr<CoordHandle> handle, ErrorHandlerCallback error_handler,
	    JournalLogger &journal, TrailerId tr)
	    : CalxCoordBaseAction::CalxCoordBaseAction(handle, error_handler,
	                                               journal),
	      trailer(tr) {}

	ErrorCode CalxCoordActionMeasure::perform(SystemManager &sysman) {
		ResourceSession session(*this->handle);
		Info(this->journal) << "Measuring coordinate plane #" << handle->getID()
		                    << " to the trailer #" << static_cast<int>(trailer);
		ErrorCode errcode = handle->measure(trailer);
		this->error_handler(errcode);
		coordActionFinished(*handle, errcode, this->journal);
		return errcode;
	}

	void CalxCoordActionMeasure::stop() {
		this->handle->stop();
		coordActionStopped(*handle, this->journal);
	}

	CalxCoordActionConfigure::CalxCoordActionConfigure(
	    std::shared_ptr<CoordHandle> handle, ErrorHandlerCallback error_handler,
	    JournalLogger &journal, PlaneMapper &mapper, coord_point_t dest,
	    double speed)
	    : CalxCoordBaseAction::CalxCoordBaseAction(handle, error_handler,
	                                               journal),
	      mapper(mapper),
	      dest(dest),
	      speed(speed),
	      work(false) {}

	ErrorCode CalxCoordActionConfigure::perform(SystemManager &sysman) {
		ResourceSession session(*this->handle);
		Info(this->journal) << "Configuring coordinate plane #" << handle->getID()
		                    << " with center " << dest.x << "x" << dest.y
		                    << " and movement speed " << speed;
		work = true;
		motor_point_t offset = { 0, 0 };
		this->mapper.setOffset(offset);
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
			this->mapper.setOffset(handle->getPosition());
		}
		this->error_handler(errcode);
		coordActionFinished(*handle, errcode, this->journal);
		return errcode;
	}

	void CalxCoordActionConfigure::stop() {
		this->work = false;
		handle->stop();
		coordActionStopped(*handle, this->journal);
	}

	CalxCoordActionGraphBuild::CalxCoordActionGraphBuild(
	    std::shared_ptr<CoordHandle> handle, ErrorHandlerCallback error_handler,
	    JournalLogger &journal, std::shared_ptr<CoordTranslator> trans,
	    std::unique_ptr<GraphBuilder> builder, double speed)
	    : CalxCoordBaseAction::CalxCoordBaseAction(handle, error_handler,
	                                               journal),
	      translator(trans),
	      builder(std::move(builder)),
	      speed(speed) {
		this->state = std::make_shared<TaskState>();
		this->state->plane = nullptr;
		this->state->work = false;
	}

	ErrorCode CalxCoordActionGraphBuild::perform(SystemManager &sysman) {
		ResourceSession session(*this->handle);
		Info(this->journal) << "Plotting graph on coordinate plane #"
		                    << handle->getID() << " function " << *builder
		                    << " with speed " << speed;
		ErrorCode errcode = builder->floatBuild(sysman, handle->getFloatPlane(),
		                                        *translator, speed, *state);
		this->error_handler(errcode);
		coordActionFinished(*handle, errcode, this->journal);
		return errcode;
	}

	void CalxCoordActionGraphBuild::stop() {
		this->state->stop();
		coordActionStopped(*handle, this->journal);
	}
}  // namespace CalX
