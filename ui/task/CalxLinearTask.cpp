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

#include "ui/task/CalxLinearTask.h"

namespace CalXUI {

	CalxLinearTaskHandle::CalxLinearTaskHandle(wxWindow *win, wxWindowID id,
	                                           std::size_t tid)
	    : CalxTaskHandle(win, id) {
		this->id = tid;
		std::string units = wxGetApp().getUnitProcessor().getUnits();

		ConfiguationFlatDictionary *confEntry =
		    wxGetApp().getSystemManager()->getConfiguration().getEntry(
		        CalxConfiguration::LinearTask);
		coord_rect_t rect = {
			confEntry->getReal(CalxLinearTaskConfiguration::XStart, 0.0),
			confEntry->getReal(CalxLinearTaskConfiguration::YStart, 0.0),
			confEntry->getReal(CalxLinearTaskConfiguration::Width, 1000.0),
			confEntry->getReal(CalxLinearTaskConfiguration::Height, 1000.0)
		};
		double spac = confEntry->getReal(CalxLinearTaskConfiguration::Spacing, 1.0);
		bool vert = confEntry->getBool(CalxLinearTaskConfiguration::Vertical, true);

		motor_rect_t mrect = { 0, 0, 0, 0 };
		motor_coord_t mspac = 0;
		bool mvert = true;
		this->task = std::make_shared<LinearCoordTask>(mrect, mspac, mvert);
		coord_point_t offset = wxGetApp().getUnitProcessor().getUnitOffset();
		coord_scale_t size = { wxGetApp().getUnitProcessor().getUnitScale(),
			                     wxGetApp().getUnitProcessor().getUnitScale() };
		this->trans = std::make_shared<LinearCoordTranslator>(offset, size);

		this->xCoord = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, INT_MIN, INT_MAX, rect.x,
		    wxGetApp().getUnitProcessor().getUnitPrecision());
		this->yCoord = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, INT_MIN, INT_MAX, rect.y,
		    wxGetApp().getUnitProcessor().getUnitPrecision());
		this->wDim =
		    new wxSpinCtrlDouble(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
		                         wxDefaultSize, wxSP_ARROW_KEYS, 1, INT_MAX, rect.w,
		                         wxGetApp().getUnitProcessor().getUnitPrecision());
		this->hDim =
		    new wxSpinCtrlDouble(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
		                         wxDefaultSize, wxSP_ARROW_KEYS, 1, INT_MAX, rect.h,
		                         wxGetApp().getUnitProcessor().getUnitPrecision());
		this->spacing = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, wxGetApp().getUnitProcessor().getUnitPrecision(),
		    INT_MAX, spac, wxGetApp().getUnitProcessor().getUnitPrecision());
		this->vertical = new wxCheckBox(this, wxID_ANY, __("Vertical"));
		this->vertical->SetValue(vert);

		wxFlexGridSizer *sizer = new wxFlexGridSizer(3);
		this->SetSizer(sizer);

		sizer->Add(new wxStaticText(this, wxID_ANY, __("x") + std::string(":")), 0,
		           wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(xCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, units));
		sizer->Add(new wxStaticText(this, wxID_ANY, __("y") + std::string(":")), 0,
		           wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(yCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, units));
		sizer->Add(new wxStaticText(this, wxID_ANY, __("Width") + std::string(":")),
		           0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(wDim);
		sizer->Add(new wxStaticText(this, wxID_ANY, units));
		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("Height") + std::string(":")), 0,
		    wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(hDim);
		sizer->Add(new wxStaticText(this, wxID_ANY, units));
		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("Spacing") + std::string(":")), 0,
		    wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(spacing);
		sizer->Add(new wxStaticText(this, wxID_ANY, units));
		sizer->Add(new wxStaticText(this, wxID_ANY, ""), 0, wxALIGN_RIGHT | wxRIGHT,
		           5);
		sizer->Add(vertical);

		this->xCoord->Bind(wxEVT_SPINCTRL, &CalxLinearTaskHandle::OnFieldChange,
		                   this);
		this->yCoord->Bind(wxEVT_SPINCTRL, &CalxLinearTaskHandle::OnFieldChange,
		                   this);
		this->wDim->Bind(wxEVT_SPINCTRL, &CalxLinearTaskHandle::OnFieldChange,
		                 this);
		this->hDim->Bind(wxEVT_SPINCTRL, &CalxLinearTaskHandle::OnFieldChange,
		                 this);
		this->spacing->Bind(wxEVT_SPINCTRL, &CalxLinearTaskHandle::OnFieldChange,
		                    this);
		this->vertical->Bind(wxEVT_CHECKBOX, &CalxLinearTaskHandle::OnFieldChange,
		                     this);
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxLinearTaskHandle::OnExit, this);

		Layout();
		this->update();
	}

	std::string CalxLinearTaskHandle::getName() const {
		return "Linear Task #" + std::to_string(this->id);
	}

	std::shared_ptr<CoordTask> CalxLinearTaskHandle::getTask() {
		return this->task;
	}

	std::shared_ptr<CoordTranslator> CalxLinearTaskHandle::getTranslator() {
		return this->trans;
	}

	void CalxLinearTaskHandle::update() {
		coord_rect_t rect = { this->xCoord->GetValue(), this->yCoord->GetValue(),
			                    this->wDim->GetValue(), this->hDim->GetValue() };
		double spacing = this->spacing->GetValue();
		bool vert = this->vertical->GetValue();

		double scale = wxGetApp().getUnitProcessor().getUnitScale();
		motor_rect_t mrect = {
			static_cast<motor_coord_t>(rect.x * scale),
			static_cast<motor_coord_t>(rect.y * scale),
			static_cast<motor_coord_t>(rect.w * scale),
			static_cast<motor_coord_t>(rect.h * scale),
		};
		motor_coord_t mspacing = static_cast<motor_coord_t>(spacing * scale);

		this->task->setRectangle(mrect);
		this->task->setSpacing(mspacing);
		this->task->setVertical(vert);
	}

	void CalxLinearTaskHandle::setRectangle(coord_rect_t rect) {
		double scale = wxGetApp().getUnitProcessor().getUnitScale();
		motor_rect_t mrect = {
			static_cast<motor_coord_t>(rect.x * scale),
			static_cast<motor_coord_t>(rect.y * scale),
			static_cast<motor_coord_t>(rect.w * scale),
			static_cast<motor_coord_t>(rect.h * scale),
		};
		this->task->setRectangle(mrect);
		this->xCoord->SetValue(rect.x);
		this->yCoord->SetValue(rect.y);
		this->wDim->SetValue(rect.w);
		this->hDim->SetValue(rect.h);
	}

	void CalxLinearTaskHandle::OnFieldChange(wxCommandEvent &evt) {
		update();
	}

	void CalxLinearTaskHandle::OnExit(wxCloseEvent &evt) {
		Destroy();
	}

	void CalxLinearTaskHandle::dump(std::ostream &out) const {
		coord_rect_t rect = { this->xCoord->GetValue(), this->yCoord->GetValue(),
			                    this->wDim->GetValue(), this->hDim->GetValue() };
		double spacing = this->spacing->GetValue();
		bool vert = this->vertical->GetValue();
		out << "Linear task \'" << this->getName() << "\' - rectangle " << rect.x
		    << 'x' << rect.y << " with area " << rect.w << 'x' << rect.h
		    << "; space between lines " << spacing << "; "
		    << (vert ? "vertical" : "horizontal") << " lines";
	}

	CalxTaskHandle *CalxLinearTaskFactory::newTask(wxWindow *win) {
		return new CalxLinearTaskHandle(win, wxID_ANY, ++this->next_id);
	}
}  // namespace CalXUI