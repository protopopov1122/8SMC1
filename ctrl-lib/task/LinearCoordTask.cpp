/*
    Copyright (c) 2017 Jevgenijs Protopopovs

    This file is part of CalX project.

    CalX is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CalX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "CoordTask.h"

namespace CalX {
	
	LinearCoordTask::LinearCoordTask(motor_rect_t square, motor_coord_t spacing, bool vertical)
		: CoordTask::CoordTask(CoordTaskType::LinearTask) {
		
		this->square = square;
		this->spacing = spacing;
		this->vertical = vertical;
	}
	
	LinearCoordTask::~LinearCoordTask() {
		
	}
	
	ErrorCode LinearCoordTask::perform(CoordPlane *plane, TaskParameters &prms, SystemManager *sysman, TaskState *state) {
		state->work = true;
		float speed = prms.speed;
		
		motor_coord_t start = this->vertical ? this->square.x : this->square.y;
		motor_coord_t end = start + (this->vertical ? this->square.w : this->square.h);
		motor_coord_t top = this->vertical ? this->square.y : this->square.x;
		motor_coord_t bottom = top + (this->vertical ? this->square.h : this->square.w);
		
		motor_point_t first, second;
		ErrorCode errcode = ErrorCode::NoError;
		for (motor_coord_t i = this->square.x; i < end && state->work; i += this->spacing) {
			if (vertical) {
				first = {i, start};
				second = {i, end};
			} else {
				first = {start, i};
				second = {end, i};
			}
			errcode = plane->move(first, speed, 8, false);
			if (errcode != ErrorCode::NoError) {
				return errcode;
			}
			errcode = plane->move(second, speed, 8, true);
			if (errcode != ErrorCode::NoError) {
				return errcode;
			}
		}
		return errcode;
	}
	
	motor_rect_t LinearCoordTask::getRectangle() {
		return this->square;
	}
	
	void LinearCoordTask::setRectangle(motor_rect_t sq) {
		this->square = sq;
	}
	
	motor_coord_t LinearCoordTask::getSpacing() {
		return this->spacing;
	}
	
	void LinearCoordTask::setSpacing(motor_coord_t sp) {
		this->spacing = sp;
	}
	
	bool LinearCoordTask::isVertical() {
		return this->vertical;
	}
	
	void LinearCoordTask::setVertical(bool v) {
		this->vertical = v;
	}
}