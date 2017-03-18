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


#include <math.h>
#include <limits>
#include <cinttypes>
#include "ctrl-lib/SystemManager.h"

namespace CalX {
	
	GraphBuilder::GraphBuilder(Node *nd, coord_point_t min, coord_point_t max, double step) {
		this->node = nd;
		this->min = min;
		this->max = max;
		this->step = step;
	}
	
	GraphBuilder::~GraphBuilder() {
		delete this->node;
	}
	
	Node *GraphBuilder::getFunction() {
		return this->node;
	}
	
	coord_point_t GraphBuilder::getMinimum() {
		return this->min;
	}
	
	coord_point_t GraphBuilder::getMaximum() {
		return this->max;
	}
	
	double GraphBuilder::getStep() {
		return this->step;
	}
	
	ErrorCode GraphBuilder::build(SystemManager *sysman, CoordPlane *plane, CoordTranslator *trans, float speed, TaskState *state) {
		plane->use();
		FunctionEngine *engine = sysman->getFunctionEngine();
		double nan = std::numeric_limits<double>::quiet_NaN();
		double last = nan;
		ErrorCode errcode;
		state->work = true;
		state->plane = plane;
		double step = fabs(this->step) * (this->max.x > this->min.x ? 1 : -1);
		for (double x = this->min.x; (step > 0 ? x <= this->max.x : x >= this->max.x) && state->work; x += step) {
			engine->getScope()->putVariable("x", x);
			engine_value_t val = engine->eval(this->node);
			if (val.err != MathError::MNoError) {
				plane->unuse();
				state->work = false;
				return ErrorCode::MathExprError;
			}
			double y = val.value;
			if (y > this->max.y || y < this->min.y) {
				y = nan;
			}
			if (isnan(y)) {
				last = y;
				continue;
			}
			motor_point_t pnt = trans->get(x, y);
			if (isnan(last)) {
				errcode = plane->move(pnt, speed, 8, false);
			} else {
				errcode = plane->move(pnt, speed, 8, true);
			}
			if (errcode != ErrorCode::NoError) {
				plane->unuse();
				state->work = false;
				return errcode;
			}
			last = y;
		}
		state->work = false;
		plane->unuse();
		return ErrorCode::NoError;
	}
}