#include <math.h>
#include <limits>
#include <cinttypes>
#include "ctrl-lib/SystemManager.h"

namespace _8SMC1 {
	
	GraphBuilder::GraphBuilder(Node *nd, coord_point_t min, coord_point_t max, long double step) {
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
	
	long double GraphBuilder::getStep() {
		return this->step;
	}
	
	ErrorCode GraphBuilder::build(SystemManager *sysman, CoordPlane *plane, CoordTranslator *trans, float speed) {
		plane->use();
		FunctionEngine *engine = sysman->getFunctionEngine();
		long double nan = std::numeric_limits<long double>::quiet_NaN();
		long double last = nan;
		ErrorCode errcode;
		long double step = fabs(this->step) * (this->max.x > this->min.x ? 1 : -1);
		for (long double x = this->min.x; (step > 0 ? x <= this->max.x : x >= this->max.x); x += step) {
			engine->getScope()->putVariable("x", x);
			engine_value_t val = engine->eval(this->node);
			if (val.err != MathError::MNoError) {
				plane->unuse();
				return ErrorCode::MathExprError;
			}
			long double y = val.value;
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
				return errcode;
			}
			last = y;
		}
		plane->unuse();
		return ErrorCode::NoError;
	}
}