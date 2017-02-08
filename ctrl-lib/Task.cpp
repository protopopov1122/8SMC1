#include "Task.h"
#include <cinttypes>

namespace Controller {

	Task::Task() {

	}

	Task::~Task() {
		for (size_t i = 0; i < this->list.size(); i++) {
			delete this->list.at(i);
		}
	}

	void Task::perform(DeviceController *ctrl) {
		for (size_t i = 0; i < this->list.size(); i++) {
			this->list.at(i)->perform(ctrl);
		}
	}

	void Task::addStep(TaskStep *st) {
		this->list.push_back(st);
	}

	JumpTaskStep::JumpTaskStep(motor_point_t pos) {
		this->pos = pos;
	}

	JumpTaskStep::~JumpTaskStep() {

	}

	void JumpTaskStep::perform(DeviceController *ctrl) {
		// TODO
	}
}
