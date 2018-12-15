#include "ui/math/CalxMathPanel.h"
#include <wx/sizer.h>
#include <wx/splitter.h>
#include <iostream>

namespace CalXUI {

	CalxMathPanel::CalxMathPanel(wxWindow *parent, wxWindowID id)
	    : CalxPanelPane::CalxPanelPane(parent, id) {
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		this->SetSizer(sizer);
		wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY);
		sizer->Add(splitter, 1, wxALL | wxEXPAND);

		wxPanel *mathObjectListPanel = new wxPanel(splitter, wxID_ANY);
		wxBoxSizer *mathObjectListSizer = new wxBoxSizer(wxVERTICAL);
		mathObjectListPanel->SetSizer(mathObjectListSizer);
		this->mathObjectList = new wxListBox(mathObjectListPanel, wxID_ANY);
		mathObjectListSizer->Add(this->mathObjectList, 1, wxALL | wxEXPAND);
		this->mathObjectList->Bind(wxEVT_LISTBOX, &CalxMathPanel::OnListClick,
		                           this);

		this->mathObjectPanel = new wxPanel(splitter, wxID_ANY);
		wxBoxSizer *mathObjectSizer = new wxBoxSizer(wxVERTICAL);
		mathObjectPanel->SetSizer(mathObjectSizer);

		splitter->Initialize(mathObjectListPanel);
		splitter->SplitVertically(mathObjectListPanel, mathObjectPanel);
		splitter->SetSashGravity(0.1f);
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
		this->Layout();
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
}  // namespace CalXUI