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

#include "ctrl-lib/ActionQueue.h"
#include "ctrl-lib/SystemManager.h"

namespace CalX {
	
	struct CalxActionResult::ResultHandle {
		ResultHandle() : status(CalxActionStatus::Pending), errcode(ErrorCode::NoError) {}
		CalxActionStatus status;
		ErrorCode errcode;
	};

	CalxActionResult::CalxActionResult() {
		this->handle = std::make_shared<ResultHandle>();
	}

	void CalxActionResult::wait() const {
		while (this->handle->status != CalxActionStatus::Finished && this->handle->status != CalxActionStatus::Stopped) {}
	}

	CalxActionStatus CalxActionResult::getStatus() const {
		return this->handle->status;
	}

	ErrorCode CalxActionResult::getError() const {
		return this->handle->errcode;
	}
	
	void CalxActionResult::update(CalxActionStatus status, ErrorCode errcode) {
		if (static_cast<int>(status) > static_cast<int>(this->handle->status)) {
			this->handle->status = status;
			this->handle->errcode = errcode;
		}
	}

	CalxActionQueue::CalxActionQueue(SystemManager &sysman, std::function<void()> queueUpdate)
	    : queueUpdate(std::move(queueUpdate)), sysman(sysman) {
		this->work.store(true);
		this->current = nullptr;
	}

	CalxActionQueue::~CalxActionQueue() {
		this->stop();
		std::unique_lock<std::mutex> lock(this->mutex);
		while (this->queue.size() > 0) {
			CalxActionHandle &handle = this->queue.at(0);
			handle.result.update(CalxActionStatus::Stopped);
			this->queue.erase(this->queue.begin());
		}
	}

	CalxActionResult CalxActionQueue::addAction(std::unique_ptr<CalxAction> act) {
		std::unique_lock<std::mutex> lock(this->mutex);
		CalxActionResult result;
		CalxActionHandle handle = { std::move(act), result };
		this->queue.push_back(std::move(handle));
		lock.unlock();
		this->queueUpdate();
		this->cond.notify_all();
		return result;
	}

	bool CalxActionQueue::isBusy() {
		return this->current != nullptr;
	}

	void CalxActionQueue::stop() {
		this->work.store(false);
		if (this->current != nullptr) {
			current->action->stop();
			current->result.update(CalxActionStatus::Stopped);
		}
		this->cond.notify_all();
	}

	void CalxActionQueue::stopCurrent() {
		if (this->current != nullptr) {
			current->action->stop();
			current->result.update(CalxActionStatus::Stopped);
		}
	}

	void CalxActionQueue::start() {
		if (this->thread.get_id() == std::thread::id()) {
			this->thread = std::thread([this]() {
				this->entry();
			});
			this->thread.detach();
		}
	}

	void CalxActionQueue::entry() {
		while (work.load()) {
			while (!this->queue.empty() && work.load()) {
				std::unique_lock<std::mutex> lock(this->mutex);
				CalxActionHandle handle = std::move(this->queue.at(0));
				this->queue.erase(this->queue.begin());
				CalxAction *action = handle.action.get();
				this->current = &handle;
				lock.unlock();
				handle.result.update(CalxActionStatus::Processing);
				ErrorCode errcode = action->perform(sysman);
				this->current = nullptr;
				handle.result.update(CalxActionStatus::Finished, errcode);
				this->queueUpdate();
			}
			std::unique_lock<std::mutex> lock(this->condMutex);
			cond.wait(lock, [&]() {
				return !this->queue.empty() || !this->work.load();
			});
		}
	}
}  // namespace CalX
