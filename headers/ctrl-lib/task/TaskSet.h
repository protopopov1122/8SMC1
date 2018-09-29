#ifndef CALX_CTRL_LIB_TASK_TASK_SET_H_
#define CALX_CTRL_LIB_TASK_TASK_SET_H_

#include "ctrl-lib/task/AbstractCoordTask.h"
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
		virtual void taskAdded(std::shared_ptr<CoordTask>) = 0;
		virtual void taskRemoved(std::size_t) = 0;
	};
	
	class VectorTaskSet : public TaskSet {
	 public:
		VectorTaskSet(TaskSetEventListener *);
		std::size_t getTaskCount() const override;
		std::weak_ptr<CoordTask> getTask(std::size_t) const override;
		std::size_t addTask(std::shared_ptr<CoordTask>) override;
		bool removeTask(std::size_t) override;
	 private:
		std::vector<std::shared_ptr<CoordTask>> tasks;
		TaskSetEventListener *listener;
	};
}

#endif