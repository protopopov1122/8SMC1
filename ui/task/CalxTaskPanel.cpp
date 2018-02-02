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

#include "ui/task/CalxTaskPanel.h"
#include "ctrl-lib/gcode/GCodeWriter.h"
#include "ui/CalxApp.h"
#include "ui/CalxErrorHandler.h"
#include "ui/coord/CalxVirtualPlane.h"
#include "ui/task/CalxGCodeTask.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <wx/sizer.h>
#include <wx/splitter.h>
#include <wx/stattext.h>
#include "ui/coord/CalxPlaneList.h"

namespace CalXUI {

	wxDEFINE_EVENT(wxEVT_TASK_PANEL_ENABLE, wxThreadEvent);

	class CalxTaskAction : public CalxAction {
	 public:
		CalxTaskAction(CalxTaskPanel *panel, std::shared_ptr<CoordHandle> handle,
		               std::shared_ptr<CoordTask> task, TaskParameters prms) {
			this->panel = panel;
			this->handle = handle;
			this->task = task;
			this->prms = prms;
			this->prms.speed *= this->handle->getFloatPlane()->getSpeedScale();
			this->state = std::make_shared<TaskState>();
			this->state->plane = nullptr;
			this->state->work = false;
		}

		virtual void perform(SystemManager *sysman) {
			handle->open_session();
			panel->setEnabled(false);
			wxGetApp().getErrorHandler()->handle(
			    task->perform(handle, prms, sysman, state));
			panel->setEnabled(true);
			handle->close_session();
		}

		virtual void stop() {
			state->stop();
		}

	 private:
		CalxTaskPanel *panel;
		std::shared_ptr<CoordHandle> handle;
		std::shared_ptr<CoordTask> task;
		TaskParameters prms;
		std::shared_ptr<TaskState> state;
	};

	class CalxPreviewAction : public CalxAction {
	 public:
		CalxPreviewAction(CalxTaskPanel *panel, CalxVirtualPlaneDialog *dialog,
		                  std::shared_ptr<CoordTask> task, TaskParameters prms) {
			this->panel = panel;
			this->dialog = dialog;
			this->task = task;
			this->prms = prms;
			this->prms.speed *= dialog->getFloatPlane()->getSpeedScale();
			this->state = std::make_shared<TaskState>();
			this->state->plane = nullptr;
			this->state->work = false;
		}

		virtual void perform(SystemManager *sysman) {
			dialog->setEnabled(false);
			panel->setEnabled(false);
			wxGetApp().getErrorHandler()->handle(
			    task->perform(dialog->getPlane(), prms, sysman, state));
			panel->setEnabled(true);
			dialog->setEnabled(true);
			dialog->Refresh();
		}

		virtual void stop() {
			state->stop();
		}

	 private:
		CalxTaskPanel *panel;
		CalxVirtualPlaneDialog *dialog;
		std::shared_ptr<CoordTask> task;
		TaskParameters prms;
		std::shared_ptr<TaskState> state;
	};

	CalxTaskPanel::CalxTaskPanel(wxWindow *win, wxWindowID id)
	    : CalxPanelPane::CalxPanelPane(win, id) {
		std::string units = wxGetApp().getUnitProcessor().getUnits();
		this->queue = new CalxActionQueue(wxGetApp().getSystemManager(), this);
		this->queue->Run();
		wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
		SetSizer(sizer);
		wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY);
		sizer->Add(splitter, 1, wxALL | wxEXPAND);

		this->taskPanel = new wxPanel(splitter, wxID_ANY);
		wxBoxSizer *taskSizer = new wxBoxSizer(wxVERTICAL);
		taskPanel->SetSizer(taskSizer);
		this->taskList = new wxListBox(taskPanel, wxID_ANY);
		taskSizer->Add(this->taskList, 1, wxALL | wxEXPAND);

		this->taskFactoryPanel = new wxPanel(taskPanel, wxID_ANY);
		taskSizer->Add(this->taskFactoryPanel, 0, wxALL | wxEXPAND);
		wxBoxSizer *taskFactorySizer = new wxBoxSizer(wxVERTICAL);
		taskFactoryPanel->SetSizer(taskFactorySizer);

		wxButton *removeButton = new wxButton(taskPanel, wxID_ANY, __("Remove"));
		taskSizer->Add(removeButton, 0, wxALL | wxEXPAND);
		removeButton->Bind(wxEVT_BUTTON, &CalxTaskPanel::OnRemoveClick, this);

		this->mainPanel = new wxPanel(splitter, wxID_ANY);
		wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
		mainPanel->SetSizer(mainSizer);

		this->execPanel = new wxPanel(this->mainPanel, wxID_ANY);
		mainSizer->Add(execPanel, 0, wxALL | wxEXPAND, 5);
		wxBoxSizer *execSizer = new wxBoxSizer(wxHORIZONTAL);
		execPanel->SetSizer(execSizer);
		wxButton *buildButton = new wxButton(execPanel, wxID_ANY, __("Build"));
		execSizer->Add(buildButton, 0, wxALL | wxALIGN_CENTER);
		this->plane = new wxChoice(execPanel, wxID_ANY);
		this->speed = new wxSpinCtrlDouble(execPanel, wxID_ANY, wxEmptyString,
		                                   wxDefaultPosition, wxDefaultSize,
		                                   wxSP_ARROW_KEYS, 0,
		                                   wxGetApp()
		                                       .getSystemManager()
		                                       ->getConfiguration()
		                                       .getEntry("units")
		                                       ->getReal("unit_speed", 4000.0),
		                                   wxGetApp()
		                                       .getSystemManager()
		                                       ->getConfiguration()
		                                       .getEntry("units")
		                                       ->getReal("unit_speed", 4000.0),
		                                   wxGetApp().getUnitProcessor().getSpeedPrecision());
		execSizer->Add(new wxStaticText(execPanel, wxID_ANY, __("on")), 0,
		               wxLEFT | wxRIGHT | wxALIGN_CENTER, 5);
		execSizer->Add(plane, 0, wxALL, 5);
		execSizer->Add(new wxStaticText(execPanel, wxID_ANY, __("with speed")), 0,
		               wxLEFT | wxRIGHT | wxALIGN_CENTER, 5);
		execSizer->Add(speed, 0, wxALL, 5);
		execSizer->Add(
		    new wxStaticText(execPanel, wxID_ANY, wxGetApp().getUnitProcessor().getSpeedUnits()), 0,
		    wxLEFT | wxRIGHT | wxALIGN_CENTER, 5);
		this->stopButton = new wxButton(execPanel, wxID_ANY, __("Stop"));
		execSizer->Add(stopButton);
		wxButton *previewButton = new wxButton(execPanel, wxID_ANY, __("Preview"));
		execSizer->Add(previewButton);
		wxButton *linearizeButton =
		    new wxButton(execPanel, wxID_ANY, __("Linearize to GCode"));
		execSizer->Add(linearizeButton);
		wxButton *moveToStartButton =
		    new wxButton(execPanel, wxID_ANY, __("Move to start"));
		execSizer->Add(moveToStartButton);
		buildButton->Bind(wxEVT_BUTTON, &CalxTaskPanel::OnBuildClick, this);
		stopButton->Bind(wxEVT_BUTTON, &CalxTaskPanel::OnStopClick, this);
		previewButton->Bind(wxEVT_BUTTON, &CalxTaskPanel::OnPreviewClick, this);
		linearizeButton->Bind(wxEVT_BUTTON, &CalxTaskPanel::OnLinearizeClick, this);
		moveToStartButton->Bind(wxEVT_BUTTON, &CalxTaskPanel::OnMoveToStartClick,
		                        this);

		splitter->Initialize(mainPanel);
		splitter->SplitVertically(taskPanel, mainPanel);
		splitter->SetSashGravity(0.1f);

		Layout();
		setEnabled(true);
		this->SetScrollRate(5, 5);

		taskList->Bind(wxEVT_LISTBOX, &CalxTaskPanel::OnListClick, this);
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxTaskPanel::OnExit, this);
		this->Bind(wxEVT_TASK_PANEL_ENABLE, &CalxTaskPanel::OnEnableEvent, this);
	}

	void CalxTaskPanel::attachTaskFactory(std::string name,
	                                      CalxTaskFactory *factory) {
		wxButton *factButton = new wxButton(
		    this->taskFactoryPanel, wxID_ANY,
		    __("New") + std::string(" ") + name + std::string(" ") + __("task"));
		this->taskFactoryPanel->GetSizer()->Add(factButton, 0, wxALL | wxEXPAND);
		factButton->Bind(wxEVT_BUTTON, &CalxTaskPanel::OnNewTaskClick, this);
		this->factories[factButton] = factory;
	}

	void CalxTaskPanel::shutdown() {
		this->queue->stop();
	}

	bool CalxTaskPanel::isBusy() {
		return queue->isBusy();
	}

	void CalxTaskPanel::stop() {
		this->queue->stopCurrent();
	}

	void CalxTaskPanel::updateUI() {
		for (const auto &t : this->list) {
			t->Show(false);
		}
		if (taskList->GetSelection() != wxNOT_FOUND) {
			this->list.at((size_t) taskList->GetSelection())->Show(true);
		}
		plane->Clear();
		for (size_t i = 0; i < wxGetApp().getSystemManager()->getCoordCount();
		     i++) {
			if (wxGetApp().getSystemManager()->getCoord(i)->isUsed()) {
				continue;
			}
			std::shared_ptr<CoordHandle> handle =
			    wxGetApp().getSystemManager()->getCoord(i);
			plane->Append("Plane #" + std::to_string(handle->getID()));
		}
		if (wxGetApp().getSystemManager()->getCoordCount() > 0) {
			plane->SetSelection(0);
		}
		this->mainPanel->Layout();
		Layout();
	}

	void CalxTaskPanel::setEnabled(bool e) {
		wxThreadEvent evt(wxEVT_TASK_PANEL_ENABLE);
		evt.SetPayload(e);
		wxPostEvent(this, evt);
	}

	void CalxTaskPanel::OnExit(wxCloseEvent &evt) {
		for (const auto &kv : this->factories) {
			delete kv.second;
		}
		for (const auto &h : list) {
			h->Close(true);
		}
		Destroy();
	}

	void CalxTaskPanel::OnNewTaskClick(wxCommandEvent &evt) {
		if (this->factories.count(evt.GetEventObject()) != 0) {
			CalxTaskFactory *fact = this->factories[evt.GetEventObject()];
			CalxTaskHandle *task = fact->newTask(mainPanel);
			if (task != nullptr) {
				list.push_back(task);
				taskList->Append(task->getName());
				mainPanel->GetSizer()->Add(task, 1, wxALL | wxEXPAND, 5);
				taskList->SetSelection((int) list.size() - 1);
				Layout();
				updateUI();
			}
		}
	}

	void CalxTaskPanel::OnRemoveClick(wxCommandEvent &evt) {
		if (taskList->GetSelection() != wxNOT_FOUND) {
			size_t sel = (size_t) taskList->GetSelection();
			taskList->Delete((unsigned int) sel);
			this->list.at(sel)->Close(true);
			this->list.erase(this->list.begin() + (std::ptrdiff_t) sel);
			updateUI();
		} else {
			std::string message = __("Select task to remove");
			wxMessageDialog *dialog = new wxMessageDialog(
			    this, message, __("Warning"), wxOK | wxICON_WARNING);
			dialog->ShowModal();
			dialog->Destroy();
		}
	}

	void CalxTaskPanel::OnListClick(wxCommandEvent &evt) {
		updateUI();
	}

	void CalxTaskPanel::OnBuildClick(wxCommandEvent &evt) {
		if (taskList->GetSelection() != wxNOT_FOUND &&
		    plane->GetSelection() != wxNOT_FOUND) {
			list.at((size_t) taskList->GetSelection())->update();
			std::shared_ptr<CoordTask> task =
			    list.at((size_t) taskList->GetSelection())->getTask();
			std::shared_ptr<CoordHandle> handle =
			    wxGetApp().getSystemManager()->getCoord(
			        (size_t) plane->GetSelection());
			float speed = this->speed->GetValue();
			TaskParameters prms = { (float) speed };
			queue->addAction(
			    std::make_unique<CalxTaskAction>(this, handle, task, prms));
		} else {
			std::string message = __("Select coordinate plane");
			if (taskList->GetSelection() == wxNOT_FOUND) {
				message = __("Select task to build");
			}
			wxMessageDialog *dialog = new wxMessageDialog(
			    this, message, __("Warning"), wxOK | wxICON_WARNING);
			dialog->ShowModal();
			dialog->Destroy();
		}
	}

	void CalxTaskPanel::OnPreviewClick(wxCommandEvent &evt) {
		if (taskList->GetSelection() != wxNOT_FOUND &&
		    plane->GetSelection() != wxNOT_FOUND) {
			list.at((size_t) taskList->GetSelection())->update();
			std::shared_ptr<CoordTask> task =
			    list.at((size_t) taskList->GetSelection())->getTask();
			std::shared_ptr<CoordHandle> handle =
			    wxGetApp().getSystemManager()->getCoord(
			        (size_t) plane->GetSelection());
			if (!handle->isMeasured()) {
				wxMessageBox(__("Plane need to be measured before preview"),
				             __("Warning"), wxICON_WARNING);
				return;
			}
			TaskParameters prms = { (float) this->speed->GetValue() };
			CalxVirtualPlaneDialog *dialog =
			    new CalxVirtualPlaneDialog(this, wxID_ANY, handle, wxSize(500, 500));
			queue->addAction(
			    std::make_unique<CalxPreviewAction>(this, dialog, task, prms));
			dialog->ShowModal();
			delete dialog;
		} else {
			std::string message = __("Select coordinate plane");
			if (taskList->GetSelection() == wxNOT_FOUND) {
				message = __("Select task to build");
			}
			wxMessageDialog *dialog = new wxMessageDialog(
			    this, message, __("Warning"), wxOK | wxICON_WARNING);
			dialog->ShowModal();
			dialog->Destroy();
		}
	}

	void CalxTaskPanel::OnLinearizeClick(wxCommandEvent &evt) {
		if (taskList->GetSelection() != wxNOT_FOUND &&
		    plane->GetSelection() != wxNOT_FOUND) {
			list.at((size_t) taskList->GetSelection())->update();
			std::shared_ptr<CoordTask> task =
			    list.at((size_t) taskList->GetSelection())->getTask();
			std::shared_ptr<CoordHandle> handle =
			    wxGetApp().getSystemManager()->getCoord(
			        (size_t) plane->GetSelection());
			if (!handle->isMeasured()) {
				wxMessageBox(__("Plane need to be measured to linearize"),
				             __("Warning"), wxICON_WARNING);
				return;
			}
			TaskParameters prms = { (float) this->speed->GetValue() };

			std::stringstream ss;
			std::shared_ptr<TaskState> state = std::make_shared<TaskState>();
			std::shared_ptr<GCodeWriter> writer = std::make_shared<GCodeWriter>(
			    handle->getPosition(), handle->getSize(),
			    list.at((size_t) taskList->GetSelection())->getTranslator(), ss);
			this->setEnabled(false);
			wxGetApp().getErrorHandler()->handle(
			    task->perform(writer, prms, wxGetApp().getSystemManager(), state));
			this->setEnabled(true);
			writer->close();

			wxFileDialog *dialog =
			    new wxFileDialog(this, __("Export linearized GCode"), "", "", "",
			                     wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
			if (dialog->ShowModal() == wxID_OK) {
				std::string path = dialog->GetPath().ToStdString();
				std::ofstream out(path);
				out << ss.str();
				out.close();
			}
			ss.seekg(0);

			std::unique_ptr<CoordTranslator> translator =
			    list.at((size_t) taskList->GetSelection())
			        ->getTranslator()
			        ->clone(nullptr);
			std::shared_ptr<ComplexCoordTranslator> trans = nullptr;
			if (translator->getType() == ComplexCoord) {
				std::shared_ptr<CoordTranslator> shtrans = std::move(translator);
				trans = std::static_pointer_cast<ComplexCoordTranslator>(shtrans);
			} else {
				std::shared_ptr<CoordTranslator> shtrans = std::move(translator);
				trans = std::make_shared<ComplexCoordTranslator>(shtrans);
			}
			CalxGcodeHandle *gcodeHandle = new CalxGcodeHandle(
			    mainPanel, wxID_ANY,
			    __("Linear ") + taskList->GetStringSelection().ToStdString(), &ss,
			    trans);

			list.push_back(gcodeHandle);
			taskList->Append(__("Linear ") +
			                 taskList->GetStringSelection().ToStdString());
			mainPanel->GetSizer()->Add(gcodeHandle, 1, wxALL | wxEXPAND, 5);
			taskList->SetSelection((int) list.size() - 1);
			Layout();
			updateUI();
			dialog->Destroy();
		} else {
			std::string message = __("Select coordinate plane");
			if (taskList->GetSelection() == wxNOT_FOUND) {
				message = __("Select task to build");
			}
			wxMessageDialog *dialog = new wxMessageDialog(
			    this, message, __("Warning"), wxOK | wxICON_WARNING);
			dialog->ShowModal();
			dialog->Destroy();
		}
	}

	void CalxTaskPanel::OnStopClick(wxCommandEvent &evt) {
		this->queue->stopCurrent();
	}

	void CalxTaskPanel::OnEnableEvent(wxThreadEvent &evt) {
		bool e = evt.GetPayload<bool>();
		for (const auto &h : list) {
			h->Enable(e);
		}
		taskPanel->Enable(e);
		for (auto i = execPanel->GetChildren().begin();
		     i != execPanel->GetChildren().end(); ++i) {
			if (*i != this->stopButton) {
				(*i)->Enable(e);
			}
		}
		this->stopButton->Enable(!e);
	}

	void CalxTaskPanel::OnMoveToStartClick(wxCommandEvent &evt) {
		if (taskList->GetSelection() != wxNOT_FOUND &&
		    plane->GetSelection() != wxNOT_FOUND) {
			list.at((size_t) taskList->GetSelection())->update();
			std::shared_ptr<CoordTask> task =
			    list.at((size_t) taskList->GetSelection())->getTask();
			std::shared_ptr<CoordHandle> handle =
			    wxGetApp().getSystemManager()->getCoord(
			        (size_t) plane->GetSelection());
			std::pair<motor_point_t, bool> start =
			    task->getStartPoint(handle->getPosition(), handle->getSize(),
			                        wxGetApp().getSystemManager());
			if (start.second) {
				float scale = wxGetApp()
				                  .getSystemManager()
				                  ->getConfiguration()
				                  .getEntry("units")
				                  ->getReal("unit_scale", 1.0f);
				float unit_speed = wxGetApp()
				                       .getSystemManager()
				                       ->getConfiguration()
				                       .getEntry("units")
				                       ->getReal("unit_speed", 1.25f);
				coord_point_t dest = { start.first.x / scale, start.first.y / scale };
				wxGetApp()
				    .getMainFrame()
				    ->getPlaneList()
				    ->getPlaneHandle((size_t) plane->GetSelection())
				    ->getController()
				    ->move(dest, unit_speed, false, false);
			}
		} else {
			std::string message = __("Select coordinate plane");
			if (taskList->GetSelection() == wxNOT_FOUND) {
				message = __("Select task to calculate");
			}
			wxMessageDialog *dialog = new wxMessageDialog(
			    this, message, __("Warning"), wxOK | wxICON_WARNING);
			dialog->ShowModal();
			dialog->Destroy();
		}
	}
}  // namespace CalXUI
