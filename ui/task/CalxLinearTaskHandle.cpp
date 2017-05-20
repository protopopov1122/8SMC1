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


#include "CalxTaskHandle.h"
#include "wx/sizer.h"
#include "wx/stattext.h"

namespace CalXUI {

	CalxLinearTaskHandle::CalxLinearTaskHandle(wxWindow *win, wxWindowID id)
		: CalxTaskHandle(win, id, CalxTaskType::CalxLinear) {
		
		motor_rect_t rect = {0, 0, 1, 1};
		motor_coord_t spac = 1;
		bool vert = true;
		this->task = new LinearCoordTask(rect, spac, vert);
		
		ConfigManager *conf = wxGetApp().getSystemManager()->getConfiguration();
		motor_point_t offset = {conf->getEntry("coords")->getInt("offset_x", 0),
			conf->getEntry("coords")->getInt("offset_y", 0)};
		motor_size_t size = {conf->getEntry("coords")->getInt("scale_x", 1),
			conf->getEntry("coords")->getInt("scale_y", 1)};
		this->trans = new ComplexCoordTranslator(new BasicCoordTranslator(offset, size));
		
		this->xCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                      wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, rect.x);
		this->yCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                      wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, rect.y);
		this->wDim = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                      wxDefaultSize, wxSP_ARROW_KEYS, 1, INT_MAX, rect.w);
		this->hDim = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                      wxDefaultSize, wxSP_ARROW_KEYS, 1, INT_MAX, rect.h);
		this->spacing = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                      wxDefaultSize, wxSP_ARROW_KEYS, 1, INT_MAX, spac);
		this->vertical = new wxCheckBox(this, wxID_ANY, __("Vertical"));
		this->vertical->SetValue(vert);
		
		wxFlexGridSizer *sizer = new wxFlexGridSizer(2);
		this->SetSizer(sizer);
		
		sizer->Add(new wxStaticText(this, wxID_ANY, __("x") + std::string(":")), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(xCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, __("y") + std::string(":")), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(yCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, __("Width") + std::string(":")), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(wDim);
		sizer->Add(new wxStaticText(this, wxID_ANY, __("Height") + std::string(":")), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(hDim);
		sizer->Add(new wxStaticText(this, wxID_ANY, __("Spacing") + std::string(":")), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(spacing);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(vertical);
		
		this->xCoord->Bind(wxEVT_SPINCTRL, &CalxLinearTaskHandle::OnFieldChange, this);
		this->yCoord->Bind(wxEVT_SPINCTRL, &CalxLinearTaskHandle::OnFieldChange, this);
		this->wDim->Bind(wxEVT_SPINCTRL, &CalxLinearTaskHandle::OnFieldChange, this);
		this->hDim->Bind(wxEVT_SPINCTRL, &CalxLinearTaskHandle::OnFieldChange, this);
		this->spacing->Bind(wxEVT_SPINCTRL, &CalxLinearTaskHandle::OnFieldChange, this);
		this->vertical->Bind(wxEVT_CHECKBOX, &CalxLinearTaskHandle::OnFieldChange, this);
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxLinearTaskHandle::OnExit, this);
		
		Layout();
	}
	
	CoordTask *CalxLinearTaskHandle::getTask() {
		return this->task;
	}
	
	ComplexCoordTranslator *CalxLinearTaskHandle::getTranslator() {
		return this->trans;
	}
	
	void CalxLinearTaskHandle::update() {
		motor_rect_t rect = {this->xCoord->GetValue(), this->yCoord->GetValue(),
		                     this->wDim->GetValue(), this->hDim->GetValue()};
		motor_coord_t spacing = this->spacing->GetValue();
		bool vert = this->vertical->GetValue();
		
		this->task->setRectangle(rect);
		this->task->setSpacing(spacing);
		this->task->setVertical(vert);
	}
	
	void CalxLinearTaskHandle::OnFieldChange(wxCommandEvent &evt) {
		update();
	}
	
	void CalxLinearTaskHandle::OnExit(wxCloseEvent &evt) {
		delete this->task;
		delete this->trans;
		Destroy();
	}
}