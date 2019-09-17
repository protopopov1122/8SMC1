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

#include "calx/ui/CalxErrorHandler.h"
#include "calx/ui/coord/CalxCoordController.h"
#include "calx/ui/coord/CalxCoordActions.h"
#include <algorithm>

namespace CalX::UI {
	CalxCoordController::CalxCoordController(std::shared_ptr<CoordHandle> handle,
	                                         CalxActionQueue *queue)
	    : handle(handle), queue(queue) {
		motor_point_t plane_offset = { 0, 0 };
		motor_scale_t plane_scale = {
			wxGetApp()
			    .getSystemManager()
			    .getConfiguration()
			    .getEntry(CalxConfiguration::Units)
			    ->getReal(CalxUnitConfiguration::PlaneScale, 1.0f),
			wxGetApp()
			    .getSystemManager()
			    .getConfiguration()
			    .getEntry(CalxConfiguration::Units)
			    ->getReal(CalxUnitConfiguration::PlaneScale, 1.0f)
		};
		float plane_speed_scale =
		    wxGetApp()
		        .getSystemManager()
		        .getConfiguration()
		        .getEntry(CalxConfiguration::Units)
		        ->getReal(CalxUnitConfiguration::PlaneSpeedScale, 1.0f);
		this->unit_map = std::make_shared<CoordPlaneMap>(
		    plane_offset, plane_scale, plane_speed_scale, handle->peekPlane());
		handle->pushPlane(this->unit_map);
		motor_point_t validateMin = { INT_MIN, INT_MIN };
		motor_point_t validateMax = { INT_MAX, INT_MAX };
		this->validator = std::make_shared<CoordPlaneValidator>(
		    validateMin, validateMax,
		    wxGetApp()
		        .getSystemManager()
		        .getConfiguration()
		        .getEntry(CalxConfiguration::Core)
		        ->getInt(CalxCoordConfiguration::MaxSpeed, 4000),
		    handle->peekPlane());
		handle->pushPlane(this->validator);
		this->log = std::make_shared<CoordPlaneLog>(
		    handle->peekPlane(), std::cout,
		    "Plane #" + std::to_string(handle->getID()) + ": ", false);
		handle->pushPlane(this->log);
		motor_point_t mapOffset = { 0, 0 };
		motor_scale_t mapScale = { 1.0f, 1.0f };
		this->map = std::make_shared<CoordPlaneMap>(mapOffset, mapScale, 1,
		                                            handle->peekPlane());
		handle->pushPlane(this->map);

		coord_scale_t unit_scale = { wxGetApp().getUnitProcessor().getUnitScale(),
			                           wxGetApp().getUnitProcessor().getUnitScale() };
		float unit_speed_scale =
		    static_cast<float>(wxGetApp().getUnitProcessor().getSpeedScale());
		handle->getFloatPlane()->setScale(unit_scale);
		handle->getFloatPlane()->setSpeedScale(unit_speed_scale);

		this->error_handler = [](ErrorCode errcode) {
			wxGetApp().getErrorHandler()->handle(errcode);
		};
	}

	std::shared_ptr<CoordHandle> CalxCoordController::getHandle() {
		return this->handle;
	}

	motor_point_t CalxCoordController::getOffset() const {
		return this->map->getOffset();
	}

	motor_scale_t CalxCoordController::getScale() const {
		return this->map->getScale();
	}

	float CalxCoordController::getSpeedScale() const {
		return this->map->getSpeedScale();
	}

	void CalxCoordController::setOffset(motor_point_t m) {
		this->map->setOffset(m);
		for (const auto &l : this->listeners) {
			l->updateOffset(m);
		}
	}

	void CalxCoordController::setScale(motor_scale_t sc) {
		this->map->setScale(sc);
		for (const auto &l : this->listeners) {
			l->updateScale(sc);
		}
	}

	void CalxCoordController::setSpeedScale(float scale) {
		this->map->setSpeedScale(scale);
		for (const auto &l : this->listeners) {
			l->updateSpeed(scale);
		}
	}

	bool CalxCoordController::isLoggingActions() {
		return this->log->isLoggingActions();
	}

	void CalxCoordController::setLoggingActions(bool la) {
		this->log->setLoggingActions(la);
	}

	bool CalxCoordController::isLoggingErrors() {
		return this->log->isLoggingErrors();
	}

	void CalxCoordController::setLoggingErrors(bool le) {
		this->log->setLoggingErrors(le);
	}

	motor_point_t CalxCoordController::getValidateMinimum() {
		return this->validator->getMinimum();
	}

	void CalxCoordController::setValidateMinimum(motor_point_t mp) {
		this->validator->setMinimum(mp);
	}

	motor_point_t CalxCoordController::getValidateMaximum() {
		return this->validator->getMaximum();
	}

	void CalxCoordController::setValidateMaximum(motor_point_t mp) {
		this->validator->setMaximum(mp);
	}

	float CalxCoordController::getValidateMaxSpeed() {
		return this->validator->getMaxSpeed();
	}

	void CalxCoordController::setValidateMaxSpeed(float ms) {
		this->validator->setMaxSpeed(ms);
	}

	void CalxCoordController::addFilterListener(CalxCoordFilterListener *l) {
		this->listeners.push_back(l);
	}

	void CalxCoordController::removeFilterListener(CalxCoordFilterListener *l) {
		this->listeners.erase(
		    std::remove(this->listeners.begin(), this->listeners.end(), l),
		    this->listeners.end());
	}

	CalxActionResult CalxCoordController::move(coord_point_t dest, double speed,
	                                           bool sync, bool relative) {
		JournalLogger &journal = wxGetApp().getJournal();
		return this->queue->addAction(std::make_unique<CalxCoordActionMove>(
		    this->handle, this->error_handler, journal, dest, speed, sync,
		    relative));
	}

	CalxActionResult CalxCoordController::arc(coord_point_t dest,
	                                          coord_point_t cen, int splitter,
	                                          double speed, bool clockwise,
	                                          bool relative) {
		JournalLogger &journal = wxGetApp().getJournal();
		return this->queue->addAction(std::make_unique<CalxCoordActionArc>(
		    this->handle, this->error_handler, journal, dest, cen, splitter, speed,
		    clockwise, relative));
	}

	CalxActionResult CalxCoordController::calibrate(TrailerId tr) {
		JournalLogger &journal = wxGetApp().getJournal();
		return this->queue->addAction(std::make_unique<CalxCoordActionCalibrate>(
		    this->handle, this->error_handler, journal, tr));
	}

	CalxActionResult CalxCoordController::measure(TrailerId tr) {
		JournalLogger &journal = wxGetApp().getJournal();
		return this->queue->addAction(std::make_unique<CalxCoordActionMeasure>(
		    this->handle, this->error_handler, journal, tr));
	}

	CalxActionResult CalxCoordController::move(coord_point_t pos, double speed) {
		coord_rect_t size = handle->getFloatPlane()->getFloatSize();
		double x = (((double) size.w) * pos.x) + size.x;
		double y = (((double) size.h) * pos.y) + size.y;
		coord_point_t dest = { x, y };
		return this->move(dest, speed, false, false);
	}

	CalxActionResult CalxCoordController::configure(coord_point_t pos,
	                                                double speed) {
		JournalLogger &journal = wxGetApp().getJournal();
		return this->queue->addAction(std::make_unique<CalxCoordActionConfigure>(
		    this->handle, this->error_handler, journal, *this, pos, speed));
	}

	CalxActionResult CalxCoordController::build(
	    std::shared_ptr<CoordTranslator> trans,
	    std::unique_ptr<GraphBuilder> builder, double speed) {
		JournalLogger &journal = wxGetApp().getJournal();
		return this->queue->addAction(std::make_unique<CalxCoordActionGraphBuild>(
		    this->handle, this->error_handler, journal, trans, std::move(builder),
		    speed));
	}

	CalxActionResult CalxCoordController::preview(
	    CalxVirtualPlaneDialog *dialog, std::shared_ptr<CoordTranslator> trans,
	    std::unique_ptr<GraphBuilder> builder, double speed) {
		return this->queue->addAction(std::make_unique<CalxCoordActionGraphPreview>(
		    dialog, trans, std::move(builder), speed));
	}
}  // namespace CalX::UI
