/*
        Copyright (c) 2017-2018 Jevgenijs Protopopovs

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