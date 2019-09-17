/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calx/ui/math/CalxMathPanel.h"
#include <wx/sizer.h>
#include <wx/splitter.h>

namespace CalX::UI {

	wxDEFINE_EVENT(wxEVT_ADD_FORMULA, wxThreadEvent);
	wxDEFINE_EVENT(wxEVT_REMOVE_FORMULA, wxThreadEvent);

	struct CalxFormulaData {
		std::string title;
		std::string formula;
		std::map<std::string, std::string> variables;
	};

	CalxMathPanel::CalxMathPanel(wxWindow *parent, wxWindowID id)
	    : CalxPanelPane::CalxPanelPane(parent, id) {
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		this->SetSizer(sizer);
		wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY);
		sizer->Add(splitter, 1, wxALL | wxEXPAND);

		this->mathObjectListPanel = new wxPanel(splitter, wxID_ANY);
		wxBoxSizer *mathObjectListSizer = new wxBoxSizer(wxVERTICAL);
		this->mathObjectListPanel->SetSizer(mathObjectListSizer);
		this->mathObjectList = new wxListBox(this->mathObjectListPanel, wxID_ANY);
		mathObjectListSizer->Add(this->mathObjectList, 1, wxALL | wxEXPAND);
		this->mathObjectList->Bind(wxEVT_LISTBOX, &CalxMathPanel::OnListClick,
		                           this);

		this->mathObjectPanel = new wxPanel(splitter, wxID_ANY);
		wxBoxSizer *mathObjectSizer = new wxBoxSizer(wxVERTICAL);
		mathObjectPanel->SetSizer(mathObjectSizer);

		splitter->Initialize(mathObjectListPanel);
		splitter->SplitVertically(mathObjectListPanel, mathObjectPanel);
		splitter->SetSashGravity(0.1f);
		splitter->SetMinimumPaneSize(100);

		this->Bind(wxEVT_ADD_FORMULA, &CalxMathPanel::OnAddFormula, this);
		this->Bind(wxEVT_REMOVE_FORMULA, &CalxMathPanel::OnRemoveFormula, this);
	}

	bool CalxMathPanel::isBusy() {
		return false;
	}

	void CalxMathPanel::shutdown() {}

	void CalxMathPanel::updateUI() {
		for (CalxMathObject *obj : this->contents) {
			obj->Show(false);
		}
		if (this->mathObjectList->GetSelection() != wxNOT_FOUND) {
			this->contents
			    .at(static_cast<std::size_t>(this->mathObjectList->GetSelection()))
			    ->Show(true);
		}
		this->mathObjectPanel->Layout();
		this->mathObjectListPanel->Layout();
		this->Layout();
		this->Update();
	}

	std::size_t CalxMathPanel::getFormulaCount() {
		return this->contents.size();
	}

	void CalxMathPanel::addFormula(
	    const std::string &title, const std::string &formula,
	    const std::map<std::string, std::string> &variables) {
		CalxFormulaData data{ title, formula, variables };
		wxThreadEvent evt(wxEVT_ADD_FORMULA);
		evt.SetPayload(data);
		wxPostEvent(this, evt);
	}

	bool CalxMathPanel::removeFormula(std::size_t index) {
		if (index < this->contents.size()) {
			wxThreadEvent evt(wxEVT_REMOVE_FORMULA);
			evt.SetPayload(index);
			wxPostEvent(this, evt);
			return true;
		} else {
			return false;
		}
	}

	void CalxMathPanel::addMathObject(CalxMathObject *object) {
		this->contents.push_back(object);
		this->mathObjectPanel->GetSizer()->Add(object, 1, wxALL | wxEXPAND);
		this->mathObjectList->Append(object->getTitle());
		this->mathObjectList->SetSelection(this->contents.size() - 1);
		this->updateUI();
	}

	void CalxMathPanel::removeMathObject(std::size_t index) {
		if (index >= this->contents.size()) {
			return;
		}
		this->contents.erase(this->contents.begin() + index);
		this->mathObjectPanel->GetSizer()->Remove(static_cast<int>(index));
	}

	void CalxMathPanel::OnListClick(wxCommandEvent &evt) {
		this->updateUI();
	}

	void CalxMathPanel::OnAddFormula(wxThreadEvent &evt) {
		CalxFormulaData data = evt.GetPayload<CalxFormulaData>();
		this->addMathObject(new CalxMathFormulaPanel(this->mathObjectPanel,
		                                             wxID_ANY, data.title,
		                                             data.formula, data.variables));
	}

	void CalxMathPanel::OnRemoveFormula(wxThreadEvent &evt) {
		this->removeMathObject(evt.GetPayload<std::size_t>());
	}
}  // namespace CalX::UI