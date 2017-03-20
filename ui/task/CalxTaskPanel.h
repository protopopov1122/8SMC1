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


#ifndef CALX_UI_CALX_TASK_PANEL_H_
#define CALX_UI_CALX_TASK_PANEL_H_

#include <vector>
#include "CalxApp.h"
#include <wx/listbox.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include "CalxTaskHandle.h"
#include "CalxActionQueue.h"

namespace CalXUI {
	
	class CalxTaskHandle; // Forward referencing
	
	class CalxTaskPanel : public wxPanel {
		public:
			CalxTaskPanel(wxWindow*, wxWindowID);
			
			void updateUI();
			void stop();
			void setEnabled(bool);
			bool isBusy() {
				return !queue->isEmpty();
			}
		private:
			void OnExit(wxCloseEvent&);
			void OnNewGcodeClick(wxCommandEvent&);
			void OnNewProgrammedClick(wxCommandEvent&);
			void OnRemoveClick(wxCommandEvent&);
			void OnListClick(wxCommandEvent&);
			void OnBuildClick(wxCommandEvent&);
			void OnQueueUpdate(wxThreadEvent&);
			void OnStopClick(wxCommandEvent&);
			
			wxListBox *taskList;
			wxPanel *mainPanel;
			wxPanel *taskPanel;
			std::vector<CalxTaskHandle*> list;
			wxPanel *execPanel;
			wxButton *stopButton;
			wxChoice *plane;
			wxSpinCtrl *speed;
			CalxActionQueue *queue;
	};
}

#endif