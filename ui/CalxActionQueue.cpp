/*
    Copyright (c) 2017 Jevgenijs Protopopovs

    This file is part of CalX project.

    CalX is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CalX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <iostream>
#include <wx/event.h>
#include "CalxActionQueue.h"

wxDEFINE_EVENT(wxEVT_COMMAND_QUEUE_UPDATE, wxThreadEvent);

namespace CalXUI {
	
	CalxActionQueue::CalxActionQueue(SystemManager *sysman, wxEvtHandler *handle)
		: wxThread::wxThread(wxTHREAD_DETACHED) {
		this->mutex = new wxMutex();
		this->cond = new wxCondition(this->condMutex);
		this->handle = handle;
		this->work = true;
		this->sysman = sysman;
		this->current = nullptr;
	}
	
	CalxActionQueue::~CalxActionQueue() {
		delete this->mutex;
		delete this->cond;
		while (this->queue.size() > 0) {
			CalxAction *action = this->queue.at(0);
			this->queue.erase(this->queue.begin());
			delete action;
		}
	}
	
	void CalxActionQueue::addAction(CalxAction *act) {
		this->mutex->Lock();
		this->queue.push_back(act);
		this->mutex->Unlock();
		wxQueueEvent(handle, new wxThreadEvent(wxEVT_COMMAND_QUEUE_UPDATE));
		cond->Broadcast();
	}
	
	bool CalxActionQueue::isEmpty() {
		return this->queue.empty();
	}
	
	void CalxActionQueue::stop() {
		if (this->current != nullptr) {
			current->stop();
		}
		this->work = false;
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
				CalxAction *action = this->queue.at(0);
				this->current = action;
				action->perform(sysman);
				this->current = nullptr;
				this->queue.erase(this->queue.begin());
				delete action;
				wxQueueEvent(handle, new wxThreadEvent(wxEVT_COMMAND_QUEUE_UPDATE));
			}
			while (this->queue.empty() && work) {
				cond->Wait();
			}
		}
		return nullptr;
	}
}