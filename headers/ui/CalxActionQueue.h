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


#ifndef CALX_UI_CALX_ACTION_QUEUE_H_
#define CALX_UI_CALX_ACTION_QUEUE_H_

#include <vector>
#include "calx.h"
#include <wx/thread.h>
#include <wx/event.h>

using namespace CalX;

wxDECLARE_EVENT(wxEVT_COMMAND_QUEUE_UPDATE, wxThreadEvent);

namespace CalXUI {
	
		class CalxAction {
			public:
				virtual ~CalxAction() {};
				virtual void perform(SystemManager*) = 0;
				virtual void stop() = 0;
		};
		
		class CalxActionQueue : public wxThread {
			public:
				CalxActionQueue(SystemManager*, wxEvtHandler*);
				virtual ~CalxActionQueue();
				void addAction(CalxAction*, bool* = nullptr);
				bool isEmpty();
				void stop();
				void stopCurrent();
			protected:
				virtual ExitCode Entry();
			private:
				bool finished;
				wxEvtHandler *handle;
				wxMutex *mutex;
				wxCondition *cond;
				wxMutex condMutex;
				std::vector<std::pair<CalxAction*, bool*>> queue;
				CalxAction *current;
				bool work;
				SystemManager *sysman;
		};
}

#endif