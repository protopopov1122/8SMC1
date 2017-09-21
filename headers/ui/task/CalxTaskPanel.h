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

#ifndef CALX_UI_CALX_TASK_PANEL_H_
#define CALX_UI_CALX_TASK_PANEL_H_

#include "ui/CalxActionQueue.h"
#include "ui/CalxApp.h"
#include "ui/CalxPanelPane.h"
#include "ui/task/CalxTaskFactory.h"
#include "ui/task/CalxTaskHandle.h"
#include <vector>
#include <wx/choice.h>
#include <wx/listbox.h>
#include <wx/spinctrl.h>

namespace CalXUI {

	class CalxTaskHandle;  // Forward referencing

	wxDECLARE_EVENT(wxEVT_TASK_PANEL_ENABLE, wxThreadEvent);

	class CalxTaskPanel : public CalxPanelPane {
	 public:
		CalxTaskPanel(wxWindow *, wxWindowID);

		virtual void updateUI();
		virtual void shutdown();
		virtual bool isBusy();
		void setEnabled(bool);

		void attachTaskFactory(std::string, CalxTaskFactory *);

	 private:
		void OnExit(wxCloseEvent &);
		void OnNewTaskClick(wxCommandEvent &);
		void OnRemoveClick(wxCommandEvent &);
		void OnListClick(wxCommandEvent &);
		void OnBuildClick(wxCommandEvent &);
		void OnPreviewClick(wxCommandEvent &);
		void OnLinearizeClick(wxCommandEvent &);
		void OnQueueUpdate(wxThreadEvent &);
		void OnStopClick(wxCommandEvent &);
		void OnEnableEvent(wxThreadEvent &);

		wxListBox *taskList;
		wxPanel *mainPanel;
		wxPanel *taskPanel;
		wxPanel *taskFactoryPanel;
		std::vector<CalxTaskHandle *> list;
		std::map<wxObject *, CalxTaskFactory *> factories;
		wxPanel *execPanel;
		wxButton *stopButton;
		wxChoice *plane;
		wxSpinCtrlDouble *speed;
		CalxActionQueue *queue;
	};
}  // namespace CalXUI

#endif
