#include "ctrl-lib/task/TaskSet.h"

namespace CalX {
	
	VectorTaskSet::VectorTaskSet(TaskSetEventListener *listener)
		: listener(listener) {}

	std::size_t VectorTaskSet::getTaskCount() const {
		return this->tasks.size();
	}
	
	std::weak_ptr<CoordTask> VectorTaskSet::getTask(std::size_t index) const {
		if (index >= this->tasks.size()) {
			return std::weak_ptr<CoordTask>();
		} else {
			return this->tasks.at(index);
		}
	}
	
	std::size_t VectorTaskSet::addTask(std::shared_ptr<CoordTask> task) {
		this->tasks.push_back(task);
		if (this->listener) {
			this->listener->onTaskAdded(task);
		}
		return this->tasks.size() - 1;
	}

	bool VectorTaskSet::removeTask(std::size_t index) {
		if (index >= this->tasks.size()) {
			return false;
		}
		if (this->listener) {
			this->listener->onTaskRemoving(index);
		}
		this->tasks.erase(this->tasks.begin() + static_cast<std::ptrdiff_t>(index));
		return true;
	}
}