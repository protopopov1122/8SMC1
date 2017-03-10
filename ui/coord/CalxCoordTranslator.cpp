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


#include <wx/sizer.h>
#include <wx/stattext.h>
#include "CalxCoordTranslator.h"

namespace CalXUI {
	
	CalxCoordTranslator::CalxCoordTranslator(wxWindow *win, wxWindowID id)
		: wxPanel::wxPanel(win, id) {
		
		motor_point_t cen = {0, 0};
		motor_size_t size = {1, 1};
		this->translator = new BasicCoordTranslator(cen, size);
		
		wxFlexGridSizer *sizer = new wxFlexGridSizer(2);
		SetSizer(sizer);
		
		this->xoffset = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->yoffset = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->xscale = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 1);
		this->yscale = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 1);
		
		sizer->Add(new wxStaticText(this, wxID_ANY, "Coordinate plane:"));
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
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
	
	CoordTranslator *CalxCoordTranslator::getTranslator() {
		updateData();
		return this->translator;
	}
	
	void CalxCoordTranslator::updateData() {
		motor_point_t offset = {this->xoffset->GetValue(), this->yoffset->GetValue()};
		motor_size_t scale = {this->xscale->GetValue(), this->yscale->GetValue()};
		this->translator->setCenter(offset);
		this->translator->setScale(scale);
	}
}