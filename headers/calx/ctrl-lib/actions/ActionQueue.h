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

#ifndef CALX_CTRL_LIB_ACTIONS_ACTION_QUEUE_H_
#define CALX_CTRL_LIB_ACTIONS_ACTION_QUEUE_H_

#include "calx/ctrl-lib/CtrlCore.h"
#include <vector>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <thread>

namespace CalX {

	class SystemManager;  // Forward referencing

	class CalxAction {
	 public:
		virtual ~CalxAction() = default;
		virtual ErrorCode perform(SystemManager &) = 0;
		virtual void stop() = 0;
	};

	enum class CalxActionStatus {
		Pending = 0,
		Processing = 1,
		Finished = 2,
		Stopped = 3
	};

	class CalxActionResult {
	 public:
		CalxActionResult();
		void wait() const;
		CalxActionStatus getStatus() const;
		ErrorCode getError() const;
		void update(CalxActionStatus, ErrorCode = ErrorCode::NoError);

	 private:
		struct ResultHandle;
		std::shared_ptr<ResultHandle> handle;
	};

	struct CalxActionHandle {
		std::unique_ptr<CalxAction> action;
		CalxActionResult result;
	};

	class CalxActionQueue {
	 public:
		CalxActionQueue(SystemManager &, std::function<void()>);
		virtual ~CalxActionQueue();
		CalxActionResult addAction(std::unique_ptr<CalxAction>);
		bool isBusy();
		void stop();
		void stopCurrent();
		void start();

	 private:
		void entry();

		std::function<void()> queueUpdate;
		std::thread thread;
		std::mutex mutex;
		std::condition_variable cond;
		std::mutex condMutex;
		std::vector<CalxActionHandle> queue;
		CalxActionHandle *current;
		std::atomic<bool> work;
		SystemManager &sysman;
	};
}  // namespace CalX

#endif