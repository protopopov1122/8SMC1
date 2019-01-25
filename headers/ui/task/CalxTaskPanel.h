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

#ifndef CALX_UI_CALX_TASK_PANEL_H_
#define CALX_UI_CALX_TASK_PANEL_H_

#include "ui/CalxActionQueue.h"
#include "ui/CalxApp.h"
#include "ui/CalxPanelPane.h"
#include "ui/task/CalxTaskFactory.h"
#include "ui/task/CalxTaskHandle.h"
#include "ui/task/CalxTaskList.h"
#include <vector>
#include <wx/choice.h>
#include <wx/listbox.h>
#include <wx/spinctrl.h>

namespace CalXUI {

	class CalxTaskHandle;  // Forward referencing

	wxDECLARE_EVENT(wxEVT_TASK_PANEL_ENABLE, wxThreadEvent);
	wxDECLARE_EVENT(wxEVT_TASK_PANEL_ATTACH_TASK, wxThreadEvent);
	wxDECLARE_EVENT(wxEVT_TASK_PANEL_REMOVE_TASK, wxThreadEvent);

	class CalxTaskPanel : public CalxPanelPane, public CalxTaskList {
	 public:
		CalxTaskPanel(wxWindow *, wxWindowID);

		void updateUI() override;
		void shutdown() override;
		bool isBusy() override;
		void setEnabled(bool);
		void stop() override;
		std::size_t getTaskCount() override;
		void removeTask(std::size_t) override;
		void attachTask(const std::string &, std::shared_ptr<CalxTaskFactory>) override;

		void attachTaskFactory(const std::string &, CalxTaskFactory *);

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
		void OnMoveToStartClick(wxCommandEvent &);
		void OnAttachTask(wxThreadEvent &);
		void OnRemoveTask(wxThreadEvent &);

		void attachTaskImpl(const std::string &, CalxTaskFactory &);
		void removeTaskImpl(std::size_t);

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
