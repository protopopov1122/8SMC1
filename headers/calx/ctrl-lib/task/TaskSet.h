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

#ifndef CALX_CTRL_LIB_TASK_TASK_SET_H_
#define CALX_CTRL_LIB_TASK_TASK_SET_H_

#include "calx/ctrl-lib/task/AbstractCoordTask.h"
#include <vector>
#include <functional>

namespace CalX {

	class TaskSet {
	 public:
		virtual ~TaskSet() = default;
		virtual std::size_t getTaskCount() const = 0;
		virtual std::weak_ptr<CoordTask> getTask(std::size_t) const = 0;
		virtual std::size_t addTask(std::shared_ptr<CoordTask>) = 0;
		virtual bool removeTask(std::size_t) = 0;
	};

	class TaskSetEventListener {
	 public:
		virtual ~TaskSetEventListener() = default;
		virtual void onTaskAdded(std::shared_ptr<CoordTask>) = 0;
		virtual void onTaskRemoving(std::size_t) = 0;
	};

	class VectorTaskSet : public TaskSet {
	 public:
		VectorTaskSet(TaskSetEventListener * = nullptr);
		std::size_t getTaskCount() const override;
		std::weak_ptr<CoordTask> getTask(std::size_t) const override;
		std::size_t addTask(std::shared_ptr<CoordTask>) override;
		bool removeTask(std::size_t) override;

	 private:
		std::vector<std::shared_ptr<CoordTask>> tasks;
		TaskSetEventListener *listener;
	};
}  // namespace CalX

#endif