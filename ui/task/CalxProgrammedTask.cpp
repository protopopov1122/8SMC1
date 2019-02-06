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

#include "calx/ui/task/CalxProgrammedTask.h"

namespace CalX::UI {
	CalxProgrammedTaskHandle::CalxProgrammedTaskHandle(wxWindow *win,
	                                                   wxWindowID id,
	                                                   std::size_t tid)
	    : CalxTaskHandle::CalxTaskHandle(win, id) {
		this->task = std::make_shared<ProgrammedCoordTask>();
		this->id = tid;

		coord_point_t offset = wxGetApp().getUnitProcessor().getUnitOffset();
		coord_scale_t size = { wxGetApp().getUnitProcessor().getUnitScale(),
			                     wxGetApp().getUnitProcessor().getUnitScale() };
		this->trans = std::make_shared<LinearCoordTranslator>(offset, size);

		wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
		SetSizer(sizer);

		wxPanel *listPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(listPanel, 0, wxALL | wxEXPAND, 5);
		wxBoxSizer *listSizer = new wxBoxSizer(wxVERTICAL);
		listPanel->SetSizer(listSizer);
		listSizer->Add(new wxStaticText(listPanel, wxID_ANY,
		                                __("Step list") + std::string(":")));
		this->stepList = new wxListBox(listPanel, wxID_ANY);
		listSizer->Add(this->stepList, 1, wxALL | wxEXPAND, 0);
		stepList->Bind(wxEVT_LISTBOX, &CalxProgrammedTaskHandle::OnListClick, this);
		wxButton *moveButton =
		    new wxButton(listPanel, wxID_ANY, __("Add move step"));
		listSizer->Add(moveButton, 0, wxALL | wxEXPAND);
		moveButton->Bind(wxEVT_BUTTON, &CalxProgrammedTaskHandle::OnMoveAddClick,
		                 this);
		wxButton *jumpButton =
		    new wxButton(listPanel, wxID_ANY, __("Add jump step"));
		listSizer->Add(jumpButton, 0, wxALL | wxEXPAND);
		jumpButton->Bind(wxEVT_BUTTON, &CalxProgrammedTaskHandle::OnJumpAddClick,
		                 this);
		wxButton *arcButton = new wxButton(listPanel, wxID_ANY, __("Add arc step"));
		listSizer->Add(arcButton, 0, wxALL | wxEXPAND);
		arcButton->Bind(wxEVT_BUTTON, &CalxProgrammedTaskHandle::OnArcAddClick,
		                this);
		moveUpButton = new wxButton(listPanel, wxID_ANY, __("Move Up"));
		listSizer->Add(moveUpButton, 0, wxALL | wxEXPAND);
		moveUpButton->Bind(wxEVT_BUTTON, &CalxProgrammedTaskHandle::OnMoveUpClick,
		                   this);
		moveDownButton = new wxButton(listPanel, wxID_ANY, __("Move Down"));
		listSizer->Add(moveDownButton, 0, wxALL | wxEXPAND);
		moveDownButton->Bind(wxEVT_BUTTON,
		                     &CalxProgrammedTaskHandle::OnMoveDownClick, this);
		wxButton *removeButton =
		    new wxButton(listPanel, wxID_ANY, __("Remove step"));
		listSizer->Add(removeButton, 0, wxALL | wxEXPAND);
		removeButton->Bind(wxEVT_BUTTON, &CalxProgrammedTaskHandle::OnRemoveClick,
		                   this);

		this->mainPanel = new wxPanel(this, wxID_ANY);
		wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
		mainPanel->SetSizer(mainSizer);
		sizer->Add(mainPanel, 1, wxALL | wxEXPAND, 5);

		this->Bind(wxEVT_CLOSE_WINDOW, &CalxProgrammedTaskHandle::OnExit, this);
		Layout();
	}

	std::string CalxProgrammedTaskHandle::getName() const {
		return "Programmed Task #" + std::to_string(this->id);
	}

	std::shared_ptr<CoordTask> CalxProgrammedTaskHandle::getTask() {
		return this->task;
	}

	std::shared_ptr<CoordTranslator> CalxProgrammedTaskHandle::getTranslator() {
		return this->trans;
	}

	void CalxProgrammedTaskHandle::update() {
		for (const auto &s : this->steps) {
			s->update();
		}
	}

	void CalxProgrammedTaskHandle::updateUI() {
		this->moveUpButton->Enable(false);
		this->moveDownButton->Enable(false);
		for (const auto &s : this->steps) {
			s->Hide();
		}
		if (stepList->GetSelection() != wxNOT_FOUND) {
			this->steps.at((std::size_t) stepList->GetSelection())->Show(true);
			this->moveUpButton->Enable(stepList->GetSelection() > 0);
			this->moveDownButton->Enable(stepList->GetSelection() <
			                             (int) (task->getSubCount() - 1));
		}
		mainPanel->Layout();
		Layout();
	}

	void CalxProgrammedTaskHandle::OnListClick(wxCommandEvent &evt) {
		updateUI();
	}

	void CalxProgrammedTaskHandle::OnMoveAddClick(wxCommandEvent &evt) {
		CalxTaskLinearStepHandle *handle =
		    new CalxTaskLinearStepHandle(this->mainPanel, wxID_ANY);
		this->mainPanel->GetSizer()->Add(handle, 0, wxALL | wxEXPAND, 5);
		steps.push_back(handle);
		stepList->Append("#" + std::to_string(steps.size()) + __(" Move"));
		stepList->SetSelection((int) steps.size() - 1);
		task->addStep(handle->getTaskStep());
		updateUI();
	}

	void CalxProgrammedTaskHandle::OnJumpAddClick(wxCommandEvent &evt) {
		CalxTaskLinearJumpStepHandle *handle =
		    new CalxTaskLinearJumpStepHandle(this->mainPanel, wxID_ANY);
		this->mainPanel->GetSizer()->Add(handle, 0, wxALL | wxEXPAND, 5);
		steps.push_back(handle);
		stepList->Append("#" + std::to_string(steps.size()) + __(" Jump"));
		stepList->SetSelection((int) steps.size() - 1);
		task->addStep(handle->getTaskStep());
		updateUI();
	}

	void CalxProgrammedTaskHandle::OnArcAddClick(wxCommandEvent &evt) {
		CalxTaskArcStepHandle *handle =
		    new CalxTaskArcStepHandle(this->mainPanel, wxID_ANY);
		this->mainPanel->GetSizer()->Add(handle, 0, wxALL | wxEXPAND, 5);
		steps.push_back(handle);
		stepList->Append("#" + std::to_string(steps.size()) + __(" Arc"));
		stepList->SetSelection((int) steps.size() - 1);
		task->addStep(handle->getTaskStep());
		updateUI();
	}

	void CalxProgrammedTaskHandle::OnMoveUpClick(wxCommandEvent &evt) {
		int sel = stepList->GetSelection();
		if (sel == wxNOT_FOUND || sel == 0) {
			return;
		}

		std::shared_ptr<TaskStep> step = task->pollStep((std::size_t) sel);
		task->insertStep((std::size_t) sel - 1, step);

		CalxTaskStepHandle *handle = steps.at((std::size_t) sel);
		steps.erase(steps.begin() + sel);
		steps.insert(steps.begin() + sel - 1, handle);

		wxString lbl = stepList->GetString((unsigned int) sel);
		stepList->Delete((unsigned int) sel);
		stepList->Insert(lbl, (unsigned int) sel - 1);
		stepList->SetSelection(sel - 1);
		updateUI();
	}

	void CalxProgrammedTaskHandle::OnMoveDownClick(wxCommandEvent &evt) {
		int sel = stepList->GetSelection();
		if (sel == wxNOT_FOUND || sel == (int) (task->getSubCount() - 1)) {
			return;
		}

		std::shared_ptr<TaskStep> step = task->pollStep((std::size_t) sel);
		task->insertStep((std::size_t) sel + 1, step);

		CalxTaskStepHandle *handle = steps.at((std::size_t) sel);
		steps.erase(steps.begin() + sel);
		steps.insert(steps.begin() + sel + 1, handle);

		wxString lbl = stepList->GetString((unsigned int) sel);
		stepList->Delete((unsigned int) sel);
		stepList->Insert(lbl, (unsigned int) sel + 1);
		stepList->SetSelection(sel + 1);
		updateUI();
	}

	void CalxProgrammedTaskHandle::OnRemoveClick(wxCommandEvent &evt) {
		if (this->stepList->GetSelection() != wxNOT_FOUND) {
			task->removeStep((std::size_t) this->stepList->GetSelection());
			this->steps.at((std::size_t) this->stepList->GetSelection())->Destroy();
			this->steps.erase(this->steps.begin() + this->stepList->GetSelection());
			stepList->Delete((unsigned int) this->stepList->GetSelection());
			updateUI();
		}
	}

	void CalxProgrammedTaskHandle::OnExit(wxCloseEvent &evt) {
		Destroy();
	}

	void CalxProgrammedTaskHandle::dump(std::ostream &out) const {
		out << "Programmed task \'" << this->getName() << "\' with "
		    << this->steps.size() << " steps";
	}

	CalxTaskHandle *CalxProgrammedTaskFactory::newTask(wxWindow *win) {
		return new CalxProgrammedTaskHandle(win, wxID_ANY, ++this->next_id);
	}
}  // namespace CalX::UI