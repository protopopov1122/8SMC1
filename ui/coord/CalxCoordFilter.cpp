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


#include "CalxApp.h"
#include "CalxCoordFilter.h"
#include "CalxFrame.h"
#include "CalxPanel.h"

namespace CalXUI {

	CalxCoordFilter::CalxCoordFilter(wxWindow *win, wxWindowID id)
		: wxPanel::wxPanel(win, id) {
			
	}
	
	CalxCoordBasicFilter::CalxCoordBasicFilter(wxWindow *win, wxWindowID id)
		: CalxCoordFilter::CalxCoordFilter(win, id) {
		
		motor_point_t cen = {0, 0};
		motor_size_t size = {1, 1};
		this->translator = new BasicCoordTranslator(cen, size);
		
		wxFlexGridSizer *sizer = new wxFlexGridSizer(2);
		SetSizer(sizer);
		
		this->xoffset = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, this->translator->getCenter().x);
		this->yoffset = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, this->translator->getCenter().y);
		this->xscale = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, this->translator->getScale().w);
		this->yscale = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX,  this->translator->getScale().h);
		
		sizer->Add(new wxStaticText(this, wxID_ANY, "X offset:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(xoffset, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Y offset:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(yoffset, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "X scale:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(xscale, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Y scale:"), 0,wxRIGHT |  wxALIGN_RIGHT, 5);
		sizer->Add(yscale, 0, wxALL | wxEXPAND);
				
		Layout();
	}
	
	CoordTranslator *CalxCoordBasicFilter::getTranslator() {
		updateData();
		return this->translator;
	}
	
	void CalxCoordBasicFilter::updateData() {
		motor_point_t offset = {this->xoffset->GetValue(), this->yoffset->GetValue()};
		motor_size_t scale = {this->xscale->GetValue(), this->yscale->GetValue()};
		this->translator->setCenter(offset);
		this->translator->setScale(scale);
	}
	
	CalxCoordLinearFilter::CalxCoordLinearFilter(wxWindow *win, wxWindowID id, CoordTranslator *base)
		: CalxCoordFilter::CalxCoordFilter(win, id) {
		
		coord_point_t cen = {0, 0};
		coord_scale_t size = {1, 1};
		this->translator = new LinearCoordTranslator(cen, size, base);
		
		wxFlexGridSizer *sizer = new wxFlexGridSizer(2);
		SetSizer(sizer);
		
		this->xoffset = new wxTextCtrl(this, wxID_ANY, "0");
		this->yoffset = new wxTextCtrl(this, wxID_ANY, "0");
		this->xscale = new wxTextCtrl(this, wxID_ANY, "1");
		this->yscale = new wxTextCtrl(this, wxID_ANY, "1");
		
		sizer->Add(new wxStaticText(this, wxID_ANY, "X offset:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(xoffset, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Y offset:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(yoffset, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "X scale:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(xscale, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Y scale:"), 0,wxRIGHT |  wxALIGN_RIGHT, 5);
		sizer->Add(yscale, 0, wxALL | wxEXPAND);
				
		Layout();
	}
	
	CoordTranslator *CalxCoordLinearFilter::getTranslator() {
		updateData();
		return this->translator;
	}
	
	void CalxCoordLinearFilter::updateData() {
		double ox, oy, sx, sy;
		this->xoffset->GetValue().ToDouble(&ox);
		this->yoffset->GetValue().ToDouble(&oy);
		this->xscale->GetValue().ToDouble(&sx);
		this->yscale->GetValue().ToDouble(&sy);
		coord_point_t offset = {ox, oy};
		coord_scale_t scale = {sx, sy};
		this->translator->setOffset(offset);
		this->translator->setScale(scale);
	}
	
	CalxCoordLogarithmicFilter::CalxCoordLogarithmicFilter(wxWindow *win, wxWindowID id, CoordTranslator *base)
		: CalxCoordFilter::CalxCoordFilter(win, id) {
		
		coord_scale_t size = {0, 0};
		this->translator = new LogarithmicCoordTranslator(size, base);
		
		wxFlexGridSizer *sizer = new wxFlexGridSizer(2);
		SetSizer(sizer);
		
		this->xscale = new wxTextCtrl(this, wxID_ANY, "0");
		this->yscale = new wxTextCtrl(this, wxID_ANY, "0");
		
		sizer->Add(new wxStaticText(this, wxID_ANY, "X scale:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(xscale, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Y scale:"), 0,wxRIGHT |  wxALIGN_RIGHT, 5);
		sizer->Add(yscale, 0, wxALL | wxEXPAND);
				
		Layout();
	}
	
	CoordTranslator *CalxCoordLogarithmicFilter::getTranslator() {
		updateData();
		return this->translator;
	}
	
	void CalxCoordLogarithmicFilter::updateData() {
		double sx, sy;
		this->xscale->GetValue().ToDouble(&sx);
		this->yscale->GetValue().ToDouble(&sy);
		coord_scale_t scale = {sx, sy};
		this->translator->setScale(scale);
	}
	
	CalxCoordPolarFilter::CalxCoordPolarFilter(wxWindow *win, wxWindowID id, CoordTranslator *base)
		: CalxCoordFilter::CalxCoordFilter(win, id) {
		
		this->translator = new PolarCoordTranslator(base);
		
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Polar coordinate filter have not any parameters.\n Add other filters."));
	}
	
	CoordTranslator *CalxCoordPolarFilter::getTranslator() {
		return this->translator;		
	}
	
	CalxCoordFilterCtrl::CalxCoordFilterCtrl(wxWindow *win, wxWindowID id)
		: CalxCoordFilter::CalxCoordFilter(win, id) {
		
		wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
		SetSizer(sizer);
		
		wxPanel *listPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(listPanel, 0, wxRIGHT | wxEXPAND, 5);
		wxBoxSizer *listSizer = new wxBoxSizer(wxVERTICAL);
		listPanel->SetSizer(listSizer);
		this->filterList = new wxListBox(listPanel, wxID_ANY);
		listSizer->Add(this->filterList, 1, wxBOTTOM | wxEXPAND);
		this->filterList->Bind(wxEVT_LISTBOX, &CalxCoordFilterCtrl::OnListClick, this);
		wxButton *addLinearButton = new wxButton(listPanel, wxID_ANY, "Add Linear");
		wxButton *addLogarithmicButton = new wxButton(listPanel, wxID_ANY, "Add Logarithmic");
		wxButton *addPolarButton = new wxButton(listPanel, wxID_ANY, "Add Polar");
		this->removeButton = new wxButton(listPanel, wxID_ANY, "Remove");
		listSizer->Add(addLinearButton, 0, wxEXPAND);
		listSizer->Add(addLogarithmicButton, 0, wxEXPAND);
		listSizer->Add(addPolarButton, 0, wxEXPAND);
		listSizer->Add(removeButton, 0, wxEXPAND);
		addLinearButton->Bind(wxEVT_BUTTON, &CalxCoordFilterCtrl::OnAddLinearClick, this);
		addLogarithmicButton->Bind(wxEVT_BUTTON, &CalxCoordFilterCtrl::OnAddLogarithmicClick, this);
		addPolarButton->Bind(wxEVT_BUTTON, &CalxCoordFilterCtrl::OnAddPolarClick, this);;
		removeButton->Bind(wxEVT_BUTTON, &CalxCoordFilterCtrl::OnRemoveClick, this);
		
		this->mainPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(mainPanel, 1, wxALL | wxEXPAND);
		wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
		mainPanel->SetSizer(mainSizer);
			
		
		this->filter.push_back(new CalxCoordBasicFilter(mainPanel, wxID_ANY));
		mainSizer->Add(this->filter.at(0), 1, wxALL | wxEXPAND);
		this->trans = new ComplexCoordTranslator(this->filter.at(0)->getTranslator());
		filterList->Append("Basic");
		filterList->SetSelection(filter.size() > 0 ? filter.size() - 1 : wxNOT_FOUND);
		
		updateUI();
		Fit();
	}
	
	CoordTranslator *CalxCoordFilterCtrl::getTranslator() {
		for (const auto& f : this->filter) {
			f->getTranslator();
		}
		return this->trans;
	}
	
	void CalxCoordFilterCtrl::updateUI() {
		for (const auto& f : this->filter) {
			f->Show(false);
		}
		this->removeButton->Enable(filterList->GetSelection() != 0 && filterList->GetSelection() != wxNOT_FOUND);
		if (filterList->GetSelection() != wxNOT_FOUND) {
			this->filter.at(filterList->GetSelection())->Show(true);
		}
		Layout();
		wxGetApp().getMainFrame()->getPanel()->updateUI();
	}
	
	void CalxCoordFilterCtrl::OnListClick(wxCommandEvent &evt) {
		updateUI();
	}
	
	void CalxCoordFilterCtrl::OnAddLinearClick(wxCommandEvent &evt) {		
		CalxCoordLinearFilter *fil = new CalxCoordLinearFilter(mainPanel, wxID_ANY,
		this->trans->get(this->trans->getSize() - 1));
		this->filter.push_back(fil);
		mainPanel->GetSizer()->Add(fil, 1, wxALL | wxEXPAND);
		this->trans->add(fil->getTranslator());
		filterList->Append("Linear");
		filterList->SetSelection(filter.size() - 1);
		updateUI();
	}
	
	void CalxCoordFilterCtrl::OnAddLogarithmicClick(wxCommandEvent &evt) {		
		CalxCoordLogarithmicFilter *fil = new CalxCoordLogarithmicFilter(mainPanel, wxID_ANY,
		this->trans->get(this->trans->getSize() - 1));
		this->filter.push_back(fil);
		mainPanel->GetSizer()->Add(fil, 1, wxALL | wxEXPAND);
		this->trans->add(fil->getTranslator());
		filterList->Append("Logarithmic");
		filterList->SetSelection(filter.size() - 1);
		updateUI();
	}
	
	void CalxCoordFilterCtrl::OnAddPolarClick(wxCommandEvent &evt) {
		CalxCoordPolarFilter *fil = new CalxCoordPolarFilter(mainPanel, wxID_ANY,
		this->trans->get(this->trans->getSize() - 1));
		this->filter.push_back(fil);
		mainPanel->GetSizer()->Add(fil, 1, wxALL | wxEXPAND);
		this->trans->add(fil->getTranslator());
		filterList->Append("Polar");
		filterList->SetSelection(filter.size() - 1);
		updateUI();
	}
	
	void CalxCoordFilterCtrl::OnRemoveClick(wxCommandEvent &evt) {
		if (filterList->GetSelection() != wxNOT_FOUND) {
			size_t i = filterList->GetSelection();
			filter.at(i)->Destroy();
			filter.erase(filter.begin() + i);
			filterList->Delete(i);
			filterList->SetSelection(filter.size() > 0 ? filter.size() - 1 : wxNOT_FOUND);
			trans->remove(i);
		}
		updateUI();
	}
}