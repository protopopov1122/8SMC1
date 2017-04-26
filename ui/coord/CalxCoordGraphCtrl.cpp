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


#include "CalxCoordCtrl.h"

namespace CalXUI {
	
	
	void CalxCoordGraphCtrl::init() {
		wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
		SetSizer(sizer);
		
		
		wxPanel *graphPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(graphPanel, 0, wxRIGHT, 10);
		wxFlexGridSizer *graphSizer = new wxFlexGridSizer(2);
		graphPanel->SetSizer(graphSizer);
		
		this->expr = new wxTextCtrl(graphPanel, wxID_ANY, "x");
        this->xmin = new wxSpinCtrl(graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                    wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, -10);
        this->xmax = new wxSpinCtrl(graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                    wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 10);
        this->ymin = new wxSpinCtrl(graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                    wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, -10);
        this->ymax = new wxSpinCtrl(graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                    wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 10);
		this->step = new wxTextCtrl(graphPanel, wxID_ANY, "1");
        this->speed = new wxSpinCtrl(graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                     wxDefaultSize, wxSP_ARROW_KEYS, 0,
                                     (int) wxGetApp().getSystemManager()->getConfiguration()->
                                        getEntry("core")->getInt("maxspeed", 4000),
                                     (int) wxGetApp().getSystemManager()->getConfiguration()->
                                        getEntry("core")->getInt("maxspeed", 4000));
		this->buildButton = new wxButton(graphPanel, wxID_ANY, __("Build"));
		this->previewButton = new wxButton(graphPanel, wxID_ANY, __("Preview"));
		
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("Function ")), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(expr, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("X axis range") + std::string(":")));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("min")), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(xmin, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("max")), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(xmax, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("Y axis range") + std::string(":")));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("min")), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(ymin, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("max")), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(ymax, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("X axis step")), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(step, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("Build speed")), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(speed, 0, wxEXPAND);
		graphSizer->Add(buildButton);
		graphSizer->Add(previewButton);
		
		this->translator = new CalxCoordFilterCtrl(this, wxID_ANY);
		sizer->Add(this->translator, 1, wxALL | wxEXPAND, 5);
	}
	
	void CalxCoordGraphCtrl::OnClose(wxCloseEvent &evt) {
		delete this->translator->getTranslator();
	}
}
