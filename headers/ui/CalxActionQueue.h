/*
        Copyright (c) 2017 Jevgenijs Protopopovs

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

#ifndef CALX_UI_CALX_ACTION_QUEUE_H_
#define CALX_UI_CALX_ACTION_QUEUE_H_

#include "ui/calx.h"
#include <vector>
#include <wx/event.h>
#include <wx/thread.h>

wxDECLARE_EVENT(wxEVT_COMMAND_QUEUE_UPDATE, wxThreadEvent);

namespace CalXUI {

	struct ActionResult {
		bool ready;
		bool stopped;
		ErrorCode errcode;
	};

	class CalxAction {
	 public:
		virtual ~CalxAction() = default;
		virtual void perform(SystemManager *) = 0;
		virtual void stop() = 0;
	};

	struct CalxActionHandle {
		std::unique_ptr<CalxAction> action;
		bool *status;
	};

	class CalxActionQueue : public wxThread {
	 public:
		CalxActionQueue(SystemManager *, wxEvtHandler *);
		virtual ~CalxActionQueue();
		void addAction(std::unique_ptr<CalxAction>, bool * = nullptr);
		bool isEmpty();
		bool isBusy();
		void stop();
		void stopCurrent();

	 protected:
		virtual ExitCode Entry();

	 private:
		bool finished;
		wxEvtHandler *evtHandle;
		wxMutex *mutex;
		wxCondition *cond;
		wxMutex condMutex;
		std::vector<CalxActionHandle> queue;
		CalxAction *current;
		bool work;
		SystemManager *sysman;
	};
}  // namespace CalXUI

#endif
