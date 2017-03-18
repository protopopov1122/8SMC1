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


#include <string>
#include "CalxApp.h"
#include "CalxCoordFilter.h"
#include "CalxFrame.h"
#include "CalxPanel.h"

namespace CalXUI {

	CalxCoordFilter::CalxCoordFilter(wxWindow *win, wxWindowID id)
		: wxPanel::wxPanel(win, id) {
			
	}
	
	CalxCoordBasicFilter::CalxCoordBasicFilter(wxWindow *win, wxWindowID id, BasicCoordTranslator *tr)
		: CalxCoordFilter::CalxCoordFilter(win, id) {
		this->translator = tr;
		
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
	
	CalxCoordLinearFilter::CalxCoordLinearFilter(wxWindow *win, wxWindowID id, LinearCoordTranslator *ctrl)
		: CalxCoordFilter::CalxCoordFilter(win, id) {
		
		this->translator = ctrl;
		
		wxFlexGridSizer *sizer = new wxFlexGridSizer(2);
		SetSizer(sizer);
		
		std::cout << ctrl->getScale().x << " " << ctrl->getScale().y << std::endl;
		
		this->xoffset = new wxTextCtrl(this, wxID_ANY, std::to_string(ctrl->getOffset().x));
		this->yoffset = new wxTextCtrl(this, wxID_ANY, std::to_string(ctrl->getOffset().y));
		this->xscale = new wxTextCtrl(this, wxID_ANY, std::to_string(ctrl->getScale().x));
		this->yscale = new wxTextCtrl(this, wxID_ANY, std::to_string(ctrl->getScale().y));
		
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
	
	CalxCoordLogarithmicFilter::CalxCoordLogarithmicFilter(wxWindow *win, wxWindowID id, LogarithmicCoordTranslator *trans)
		: CalxCoordFilter::CalxCoordFilter(win, id) {
		
		this->translator = trans;
		
		wxFlexGridSizer *sizer = new wxFlexGridSizer(2);
		SetSizer(sizer);
		
		this->xscale = new wxTextCtrl(this, wxID_ANY, std::to_string(trans->getScale().x));
		this->yscale = new wxTextCtrl(this, wxID_ANY, std::to_string(trans->getScale().y));
		
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
	
	CalxCoordPolarFilter::CalxCoordPolarFilter(wxWindow *win, wxWindowID id, PolarCoordTranslator *polar)
		: CalxCoordFilter::CalxCoordFilter(win, id) {
		
		this->translator = polar;
		
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Polar coordinate filter have not any parameters.\n Add other filters."));
	}
	
	CoordTranslator *CalxCoordPolarFilter::getTranslator() {
		return this->translator;		
	}
	
	CalxCoordFilterCtrl::CalxCoordFilterCtrl(wxWindow *win, wxWindowID id, ComplexCoordTranslator *tr)
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
		
		this->trans = tr;
		
		if (this->trans == nullptr) {
			motor_point_t cen = {0, 0};
			motor_size_t scl = {1, 1};
			BasicCoordTranslator *basic = new BasicCoordTranslator(cen, scl);
			this->trans = new ComplexCoordTranslator(basic);
			addFilter(basic);
		} else {
			for (size_t i = 0; i < trans->getSize(); i++) {
				addFilter(trans->get(i));
			}
		}
		
		updateUI();
		Fit();
	}
	
	ComplexCoordTranslator *CalxCoordFilterCtrl::getTranslator() {
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
		CoordTranslator *base = this->trans->get(this->trans->getSize() - 1);
		coord_point_t offset = {0, 0};
		coord_scale_t scale = {1, 1};
		LinearCoordTranslator *linear = new LinearCoordTranslator(offset, scale, base);
		this->trans->add(linear);
		addFilter(linear);
	}
	
	void CalxCoordFilterCtrl::OnAddLogarithmicClick(wxCommandEvent &evt) {		
		CoordTranslator *base = this->trans->get(this->trans->getSize() - 1);
		coord_scale_t sc = {0, 0};
		LogarithmicCoordTranslator *logt = new LogarithmicCoordTranslator(sc, base);
		this->trans->add(logt);
		addFilter(logt);
	}
	
	void CalxCoordFilterCtrl::OnAddPolarClick(wxCommandEvent &evt) {
		CoordTranslator *base = this->trans->get(this->trans->getSize() - 1);
		PolarCoordTranslator *polar = new PolarCoordTranslator(base);
		this->trans->add(polar);
		addFilter(polar);
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
	
	void CalxCoordFilterCtrl::addFilter(CoordTranslator *tr) {
		CalxCoordFilter *fil = nullptr;
		switch (tr->getType()) {
			case CoordType::BasicCoord:
				fil = new CalxCoordBasicFilter(mainPanel, wxID_ANY, (BasicCoordTranslator*) tr);
				filterList->Append("Basic");
				break;
			case CoordType::LinearCoord:
				fil = new CalxCoordLinearFilter(mainPanel, wxID_ANY, (LinearCoordTranslator*) tr);
				filterList->Append("Linear");
				break;
			case CoordType::LogarithmicCoord:
				fil = new CalxCoordLogarithmicFilter(mainPanel, wxID_ANY, (LogarithmicCoordTranslator*) tr);
				filterList->Append("Logarithmic");
				break;
			case CoordType::PolarCoord:
				fil = new CalxCoordPolarFilter(mainPanel, wxID_ANY, (PolarCoordTranslator*) tr);
				filterList->Append("Polar");
				break;
			default:
				break;
		}
		if (fil == nullptr) {
			return;
		}
		this->filter.push_back(fil);
		mainPanel->GetSizer()->Add(fil, 1, wxALL | wxEXPAND);
		filterList->SetSelection(filter.size() - 1);
		updateUI();
		
	}
}