#include <iostream>
#include <wx/event.h>
#include "CalxActionQueue.h"

wxDEFINE_EVENT(wxEVT_COMMAND_QUEUE_UPDATE, wxThreadEvent);

namespace CalX {
	
	CalxActionQueue::CalxActionQueue(SystemManager *sysman, wxEvtHandler *handle)
		: wxThread::wxThread(wxTHREAD_DETACHED) {
		this->mutex = new wxMutex();
		this->cond = new wxCondition(this->condMutex);
		this->handle = handle;
		this->work = true;
		this->sysman = sysman;
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
		this->work = false;
	}
	
	void *CalxActionQueue::Entry() {
		while (work) {
			while (!this->queue.empty()) {
				CalxAction *action = this->queue.at(0);
				action->perform(sysman);
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