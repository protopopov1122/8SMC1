#include "CoordTask.h"
#include "SystemManager.h"
#include <cinttypes>
#include <iostream>
#include <math.h>
#include <limits>

namespace _8SMC1 {

	ProgrammedCoordTask::ProgrammedCoordTask() :
		CoordTask::CoordTask(CoordTaskType::ProgrammedTask){

	}

	ProgrammedCoordTask::~ProgrammedCoordTask() {
		for (size_t i = 0; i < this->list.size(); i++) {
			delete this->list.at(i);
		}
	}

	ErrorCode ProgrammedCoordTask::perform(CoordPlane *ctrl, TaskParameters &prms, SystemManager *sysman) {
		ErrorCode errcode;
		for (size_t i = 0; i < this->list.size(); i++) {
			errcode = this->list.at(i)->perform(ctrl, prms, sysman);
			if (errcode != ErrorCode::NoError) {
				return errcode;
			}
		}
		return ErrorCode::NoError;
	}

	void ProgrammedCoordTask::addStep(TaskStep *st) {
		this->list.push_back(st);
	}

	size_t ProgrammedCoordTask::getSubCount() {
		return this->list.size();
	}

	MoveTaskStep::MoveTaskStep(motor_point_t pos, float spc, bool rel) : TaskStep::TaskStep() {
		this->pos = pos;
		this->speed_coef = spc;
		this->rel = rel;
	}

	MoveTaskStep::~MoveTaskStep() {

	}

	ErrorCode MoveTaskStep::perform(CoordPlane *ctrl, TaskParameters &prms, SystemManager *sysman) {
		if (this->rel) {
			return ctrl->relativeMove(this->pos, this->speed_coef * prms.speed, 8, true);
		} else {
			return ctrl->move(this->pos, this->speed_coef * prms.speed, 8, true);
		}
	}

	JumpTaskStep::JumpTaskStep(motor_point_t pos, float spc, bool rel) : TaskStep::TaskStep() {
		this->pos = pos;
		this->speed_coef = spc;
		this->rel = rel;
	}

	JumpTaskStep::~JumpTaskStep() {

	}

	ErrorCode JumpTaskStep::perform(CoordPlane *ctrl, TaskParameters &prms, SystemManager *sysman) {
		if (this->rel) {
			return ctrl->relativeMove(this->pos, this->speed_coef * prms.speed, 8, false);
		} else {
			return ctrl->move(this->pos, this->speed_coef * prms.speed, 8, false);
		}
	}

	CalibrateTaskStep::CalibrateTaskStep(TrailerId side) : TaskStep::TaskStep() {
		this->side = side;
	}

	CalibrateTaskStep::~CalibrateTaskStep() {

	}

	ErrorCode CalibrateTaskStep::perform(CoordPlane *ctrl, TaskParameters &prms, SystemManager *sysman) {
		return ctrl->calibrate(this->side);
	}

	ArcTaskStep::ArcTaskStep(motor_point_t dest, motor_point_t center, int sp, float speed, bool rel)
			: TaskStep::TaskStep() {
		this->dest = dest;
		this->center = center;
		this->splitter = sp;
		this->speed = speed;
		this->rel = rel;
	}

	ArcTaskStep::~ArcTaskStep() {

	}

	ErrorCode ArcTaskStep::perform(CoordPlane *ctrl, TaskParameters &prms, SystemManager *sysman) {		
		if (this->rel) {
			return ctrl->relativeArc(dest, center, splitter, this->speed * prms.speed, 8, this->clockwise);
		} else {
			return ctrl->arc(dest, center, splitter, this->speed * prms.speed, 8, this->clockwise);
		}
	}

	void ArcTaskStep::setClockwise(bool c) {
		this->clockwise = c;
	}

	RelArcTaskStep::RelArcTaskStep(motor_point_t dest, motor_point_t center, int sp, float speed, bool rel)
			: TaskStep::TaskStep() {
		this->dest = dest;
		this->center = center;
		this->splitter = sp;
		this->speed = speed;
		this->rel = rel;
		
	}

	RelArcTaskStep::~RelArcTaskStep() {

	}

	ErrorCode RelArcTaskStep::perform(CoordPlane *ctrl, TaskParameters &prms, SystemManager *sysman) {	
		motor_point_t cen = ctrl->getPosition();
		cen.x += center.x;
		cen.y += center.y;
		if (this->rel) {
			return ctrl->relativeArc(dest, cen, splitter, this->speed * prms.speed, 8, this->clockwise);
		} else {
			return ctrl->arc(dest, cen, splitter, this->speed * prms.speed, 8, this->clockwise);
		}
	}

	void RelArcTaskStep::setClockwise(bool c) {
		this->clockwise = c;
	}
	
	GraphCoordTask::GraphCoordTask(Node *func, CoordTranslator *trans,
		coord_point_t min, coord_point_t max, long double step, float scale)
		: CoordTask::CoordTask(CoordTaskType::GraphTask) {
		this->func = func;
		this->trans = trans;
		this->min = min;
		this->max = max;
		this->step = step;
		this->scale = scale;
	}
	
	GraphCoordTask::~GraphCoordTask() {
		delete this->func;
		delete this->trans;
	}
	
	ErrorCode GraphCoordTask::perform(CoordPlane *plane, TaskParameters &prms, SystemManager *sysman) {
		FunctionEngine *engine = sysman->getFunctionEngine();
		long double nan = std::numeric_limits<long double>::quiet_NaN();
		long double last = nan;
		float speed = this->scale * prms.speed;
		ErrorCode errcode;
		for (long double x = this->min.x; x <= this->max.x; x += this->step) {
			engine->getScope()->putVariable("x", x);
			engine_value_t val = engine->eval(this->func);
			if (val.err != MathError::MNoError) {
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
			motor_point_t pnt = this->trans->get(x, y);
			if (isnan(last)) {
				errcode = plane->move(pnt, speed, 8, false);
			} else {
				errcode = plane->move(pnt, speed, 8, true);
			}
			if (errcode != ErrorCode::NoError) {
				return errcode;
			}
			last = y;
		}
		return ErrorCode::NoError;
	}
}
