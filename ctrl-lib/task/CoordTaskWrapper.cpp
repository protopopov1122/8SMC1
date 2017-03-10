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


#include <vector>
#include "CoordTask.h"

namespace CalX {
	
	CoordTaskWrapper::CoordTaskWrapper(CoordTask *task)
			: CoordTask::CoordTask(CoordTaskType::WrapperTask) {
		this->task = task;
	}
	
	CoordTaskWrapper::~CoordTaskWrapper() {
		delete this->task;
	}
	
	void CoordTaskWrapper::setCalibration(TrailerId id) {
		this->cal_tr = true;
		this->tr = id;
	}
	
	void CoordTaskWrapper::setValidation(motor_point_t min, motor_point_t max, float speed) {
		this->coord_val = true;
		this->val_min = min;
		this->val_max = max;
		this->val_speed = speed;
	}
	
	void CoordTaskWrapper::setMap(motor_point_t offset, motor_scale_t sc) {
		this->coord_map = true;
		this->coord_offset = offset;
		this->coord_scale = sc;
	}
	
	ErrorCode CoordTaskWrapper::perform(CoordPlane *plane, TaskParameters &prms, SystemManager *sysman, TaskState *state) {
		std::vector<CoordPlane*> vec;
		vec.push_back(plane);
		if (this->cal_tr) {
			plane->calibrate(this->tr);
		}
		if (this->coord_val) {
			CoordPlaneValidator *val = new CoordPlaneValidator(this->val_min, this->val_max,
				this->val_speed, vec.at(vec.size() - 1));
			vec.push_back(val);
		}
		if (this->coord_map) {
			CoordPlaneMap *map = new CoordPlaneMap(this->coord_offset, this->coord_scale,
				vec.at(vec.size() - 1));
			vec.push_back(map);
		}
		ErrorCode ec = this->task->perform(vec.at(vec.size() - 1), prms, sysman, state);
		while (vec.size() > 1) {
			CoordPlane *pl = vec.at(vec.size() - 1);
			vec.pop_back();
			delete pl;
		}
		return ec;
	}
}