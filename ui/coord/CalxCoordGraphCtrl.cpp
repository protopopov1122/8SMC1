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


#include "ui/coord/CalxCoordCtrl.h"

namespace CalXUI {


	void CalxCoordGraphCtrl::init() {
		std::string units = wxGetApp().getUnits();
		ConfigEntry *graphconf = wxGetApp().getSystemManager()->getConfiguration()->getEntry("graph");
		wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
		SetSizer(sizer);


		wxPanel *graphPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(graphPanel, 0, wxRIGHT, 10);
		wxFlexGridSizer *graphSizer = new wxFlexGridSizer(3);
		graphPanel->SetSizer(graphSizer);

		this->expr = new wxTextCtrl(graphPanel, wxID_ANY, "x");
		this->xmin = new wxSpinCtrlDouble(graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                      wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX,
									                    graphconf->getReal("x_from", -10.0),
																		  graphconf->getReal("x_axis_step", 0.001));
		this->xmax = new wxSpinCtrlDouble(graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                      wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX,
				                     					graphconf->getReal("x_to", 10.0),
									                    graphconf->getReal("x_axis_step", 0.001));
    this->ymin = new wxSpinCtrlDouble(graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                      wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX,
			                     						graphconf->getReal("y_from", -10.0),
									                    graphconf->getReal("y_axis_step", 0.001));
    this->ymax = new wxSpinCtrlDouble(graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                      wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX,
			                     						graphconf->getReal("y_to", 10.0),
									                    graphconf->getReal("y_axis_step", 0.001));
		this->step = new wxSpinCtrlDouble(graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                      wxDefaultSize, wxSP_ARROW_KEYS, graphconf->getReal("step_step", 0.001), INT_MAX,
		                     							graphconf->getReal("step", 0.1),
									                    graphconf->getReal("step_step", 0.001));
    this->speed = new wxSpinCtrlDouble(graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                     wxDefaultSize, wxSP_ARROW_KEYS, 0,
                                     wxGetApp().getSystemManager()->getConfiguration()->
                                        getEntry("units")->getReal("unit_speed", 4000.0),
                                     wxGetApp().getSystemManager()->getConfiguration()->
                                        getEntry("units")->getReal("unit_speed", 4000.0),
									 wxGetApp().getSpeedPrecision());
		this->buildButton = new wxButton(graphPanel, wxID_ANY, __("Build"));
		this->previewButton = new wxButton(graphPanel, wxID_ANY, __("Preview"));

		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("Function ")), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(expr, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(this, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("X axis range") + std::string(":")));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(this, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("min")), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(xmin, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(this, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("max")), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(xmax, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(this, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("Y axis range") + std::string(":")));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(this, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("min")), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(ymin, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(this, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("max")), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(ymax, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(this, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("X axis step")), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(step, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(this, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("Build speed")), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(speed, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(this, wxID_ANY, wxGetApp().getSpeedUnits()));
		graphSizer->Add(buildButton);
		graphSizer->Add(previewButton);


		ConfigManager *conf = wxGetApp().getSystemManager()->getConfiguration();
		coord_point_t cen = {static_cast<double>(conf->getEntry("coords")->getReal("offset_x", 0.0)),
			static_cast<double>(conf->getEntry("coords")->getReal("offset_y", 0.0))};
		coord_scale_t scl = {static_cast<double>(conf->getEntry("coords")->getReal("scale_x", 1.0)),
			static_cast<double>(conf->getEntry("coords")->getReal("scale_y", 1.0))};
		LinearCoordTranslator *basic = new LinearCoordTranslator(cen, scl);
		this->trans = new ComplexCoordTranslator(basic);
		this->translator = new CalxCoordFilterCtrl(this, wxID_ANY, this->trans);
		sizer->Add(this->translator, 1, wxALL | wxEXPAND, 5);
	}

	void CalxCoordGraphCtrl::OnClose(wxCloseEvent &evt) {
		delete this->translator->getTranslator();
	}
}
