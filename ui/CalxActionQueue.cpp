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

#include "ui/CalxActionQueue.h"
#include <iostream>
#include <wx/event.h>

wxDEFINE_EVENT(wxEVT_COMMAND_QUEUE_UPDATE, wxThreadEvent);

namespace CalXUI {

	CalxActionQueue::CalxActionQueue(SystemManager *sysman, wxEvtHandler *handle)
	    : wxThread::wxThread(wxTHREAD_DETACHED) {
		this->mutex = new wxMutex();
		this->cond = new wxCondition(this->condMutex);
		this->evtHandle = handle;
		this->work = true;
		this->sysman = sysman;
		this->current = nullptr;
		this->finished = false;
	}

	CalxActionQueue::~CalxActionQueue() {
		delete this->mutex;
		delete this->cond;
		while (this->queue.size() > 0) {
			CalxActionHandle &handle = this->queue.at(0);
			if (handle.status != nullptr) {
				*handle.status = true;
			}
			this->queue.erase(this->queue.begin());
		}
	}

	void CalxActionQueue::addAction(std::unique_ptr<CalxAction> act, bool *flag) {
		this->mutex->Lock();
		CalxActionHandle handle = { std::move(act), flag };
		this->queue.push_back(std::move(handle));
		this->mutex->Unlock();
		wxQueueEvent(evtHandle, new wxThreadEvent(wxEVT_COMMAND_QUEUE_UPDATE));
		cond->Broadcast();
	}

	bool CalxActionQueue::isEmpty() {
		return this->queue.empty();
	}

	bool CalxActionQueue::isBusy() {
		return this->current != nullptr;
	}

	void CalxActionQueue::stop() {
		finished = false;
		this->work = false;
		if (this->current != nullptr) {
			current->stop();
		}
		cond->Broadcast();
	}

	void CalxActionQueue::stopCurrent() {
		if (this->current != nullptr) {
			current->stop();
		}
	}

	void *CalxActionQueue::Entry() {
		while (work) {
			while (!this->queue.empty() && work) {
				this->mutex->Lock();
				CalxActionHandle handle = std::move(this->queue.at(0));
				this->queue.erase(this->queue.begin());
				CalxAction *action = handle.action.get();
				this->current = action;
				this->mutex->Unlock();
				action->perform(sysman);
				this->current = nullptr;
				if (handle.status != nullptr) {
					*handle.status = true;
				}
				wxQueueEvent(evtHandle, new wxThreadEvent(wxEVT_COMMAND_QUEUE_UPDATE));
			}
			while (this->queue.empty() && work) {
				cond->Wait();
			}
		}
		finished = true;
		return nullptr;
	}
}  // namespace CalXUI
