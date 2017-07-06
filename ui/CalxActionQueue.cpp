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
#include "ui/CalxActionQueue.h"
#include <wx/event.h>

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
	this->finished = false;
  }

  CalxActionQueue::~CalxActionQueue() {
	delete this->mutex;
	delete this->cond;
	while (this->queue.size() > 0) {
	  std::pair<CalxAction *, bool *> pair = this->queue.at(0);
	  CalxAction *action = pair.first;
	  this->queue.erase(this->queue.begin());
	  delete action;
	  if (pair.second != nullptr) {
		*pair.second = true;
	  }
	}
  }

  void CalxActionQueue::addAction(CalxAction *act, bool *flag) {
	this->mutex->Lock();
	this->queue.push_back(std::make_pair(act, flag));
	this->mutex->Unlock();
	wxQueueEvent(handle, new wxThreadEvent(wxEVT_COMMAND_QUEUE_UPDATE));
	cond->Broadcast();
  }

  bool CalxActionQueue::isEmpty() {
	return this->queue.empty();
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
		std::pair<CalxAction *, bool *> pair = this->queue.at(0);
		CalxAction *action = pair.first;
		this->current = action;
		this->queue.erase(this->queue.begin());
		this->mutex->Unlock();
		action->perform(sysman);
		this->current = nullptr;
		delete action;
		if (pair.second != nullptr) {
		  *pair.second = true;
		}
		wxQueueEvent(handle, new wxThreadEvent(wxEVT_COMMAND_QUEUE_UPDATE));
	  }
	  while (this->queue.empty() && work) {
		cond->Wait();
	  }
	}
	finished = true;
	return nullptr;
  }
}
