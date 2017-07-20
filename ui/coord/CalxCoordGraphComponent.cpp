/*
        Copyright (c) 2017 Jevgenijs Protopopovs

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

#include "ui/coord/CalxCoordGraphComponent.h"
#include "ctrl-lib/graph/FunctionEngine.h"
#include "ctrl-lib/graph/FunctionParser.h"
#include "ctrl-lib/misc/GraphBuilder.h"
#include "ui/CalxErrorHandler.h"
#include <sstream>

namespace CalXUI {

	CalxCoordComponent *CalxCoordGraphComponentFactory::newComponent(
	    wxWindow *win, CalxCoordController *ctrl) {
		return new CalxCoordGraphComponent(win, wxID_ANY, ctrl);
	}

	CalxCoordGraphComponent::CalxCoordGraphComponent(
	    wxWindow *win, wxWindowID id, CalxCoordController *controller)
	    : CalxCoordComponent::CalxCoordComponent(win, id),
	      controller(controller) {
		std::string units = wxGetApp().getUnits();
		std::shared_ptr<ConfigEntry> graphconf =
		    wxGetApp().getSystemManager()->getConfiguration()->getEntry("graph");
		wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
		SetSizer(sizer);

		wxPanel *graphPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(graphPanel, 0, wxRIGHT, 10);
		wxFlexGridSizer *graphSizer = new wxFlexGridSizer(3);
		graphPanel->SetSizer(graphSizer);

		this->expr = new wxTextCtrl(graphPanel, wxID_ANY, "x");
		this->xmin = new wxSpinCtrlDouble(
		    graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, INT_MIN, INT_MAX, graphconf->getReal("x_from", -10.0),
		    graphconf->getReal("x_axis_step", 0.001));
		this->xmax = new wxSpinCtrlDouble(
		    graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, INT_MIN, INT_MAX, graphconf->getReal("x_to", 10.0),
		    graphconf->getReal("x_axis_step", 0.001));
		this->ymin = new wxSpinCtrlDouble(
		    graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, INT_MIN, INT_MAX, graphconf->getReal("y_from", -10.0),
		    graphconf->getReal("y_axis_step", 0.001));
		this->ymax = new wxSpinCtrlDouble(
		    graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, INT_MIN, INT_MAX, graphconf->getReal("y_to", 10.0),
		    graphconf->getReal("y_axis_step", 0.001));
		this->step = new wxSpinCtrlDouble(
		    graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, graphconf->getReal("step_step", 0.001), INT_MAX,
		    graphconf->getReal("step", 0.1),
		    graphconf->getReal("step_step", 0.001));
		this->speed = new wxSpinCtrlDouble(graphPanel, wxID_ANY, wxEmptyString,
		                                   wxDefaultPosition, wxDefaultSize,
		                                   wxSP_ARROW_KEYS, 0,
		                                   wxGetApp()
		                                       .getSystemManager()
		                                       ->getConfiguration()
		                                       ->getEntry("units")
		                                       ->getReal("unit_speed", 4000.0),
		                                   wxGetApp()
		                                       .getSystemManager()
		                                       ->getConfiguration()
		                                       ->getEntry("units")
		                                       ->getReal("unit_speed", 4000.0),
		                                   wxGetApp().getSpeedPrecision());
		wxButton *buildButton = new wxButton(graphPanel, wxID_ANY, __("Build"));
		wxButton *previewButton = new wxButton(graphPanel, wxID_ANY, __("Preview"));

		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("Function ")), 0,
		                wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(expr, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(this, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY,
		                                 __("X axis range") + std::string(":")));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(this, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("min")), 0,
		                wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(xmin, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(this, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("max")), 0,
		                wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(xmax, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(this, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY,
		                                 __("Y axis range") + std::string(":")));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(this, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("min")), 0,
		                wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(ymin, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(this, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("max")), 0,
		                wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(ymax, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(this, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("X axis step")),
		                0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(step, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(this, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, __("Build speed")),
		                0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(speed, 0, wxEXPAND);
		graphSizer->Add(
		    new wxStaticText(this, wxID_ANY, wxGetApp().getSpeedUnits()));
		graphSizer->Add(buildButton);
		graphSizer->Add(previewButton);

		std::shared_ptr<ConfigManager> conf = wxGetApp().getSystemManager()->getConfiguration();
		coord_point_t cen = {
			static_cast<double>(conf->getEntry("coords")->getReal("offset_x", 0.0)),
			static_cast<double>(conf->getEntry("coords")->getReal("offset_y", 0.0))
		};
		coord_scale_t scl = {
			static_cast<double>(conf->getEntry("coords")->getReal("scale_x", 1.0)),
			static_cast<double>(conf->getEntry("coords")->getReal("scale_y", 1.0))
		};
		std::shared_ptr<LinearCoordTranslator> basic = std::make_shared<LinearCoordTranslator>(cen, scl);
		this->trans = std::make_shared<ComplexCoordTranslator>(basic);
		this->translator = new CalxCoordFilterCtrl(this, wxID_ANY, this->trans);
		sizer->Add(this->translator, 1, wxALL | wxEXPAND, 5);

		buildButton->Bind(wxEVT_BUTTON, &CalxCoordGraphComponent::OnBuildClick,
		                  this);
		previewButton->Bind(wxEVT_BUTTON, &CalxCoordGraphComponent::OnPreviewClick,
		                    this);
		Bind(wxEVT_CLOSE_WINDOW, &CalxCoordGraphComponent::OnClose, this);
	}

	void CalxCoordGraphComponent::OnClose(wxCloseEvent &evt) {
	}

	void CalxCoordGraphComponent::OnBuildClick(wxCommandEvent &evt) {
		std::stringstream ss(this->expr->GetValue().ToStdString());
		FunctionParser parser(std::make_unique<FunctionLexer>(ss));
		std::unique_ptr<Node> node = parser.parse();
		if (node == nullptr) {
			wxGetApp().getErrorHandler()->handle(ErrorCode::MathExprError);
			return;
		}
		double minx = xmin->GetValue();
		double maxx = xmax->GetValue();
		double miny = ymin->GetValue();
		double maxy = ymax->GetValue();
		double step = this->step->GetValue();
		double speed = this->speed->GetValue();
		coord_point_t min = { minx, miny };
		coord_point_t max = { maxx, maxy };
		std::unique_ptr<GraphBuilder> graph = std::make_unique<GraphBuilder>(std::move(node), min, max, step);
		this->controller->build(this->translator->getTranslator(), std::move(graph), speed);
	}

	void CalxCoordGraphComponent::OnPreviewClick(wxCommandEvent &evt) {
		if (!this->controller->getHandle()->isMeasured()) {
			wxMessageBox(__("Plane need to be measured before preview"),
			             __("Warning"), wxICON_WARNING);
			return;
		}
		std::stringstream ss(this->expr->GetValue().ToStdString());
		FunctionParser parser(std::make_unique<FunctionLexer>(ss));
		std::unique_ptr<Node> node = parser.parse();
		if (node == nullptr) {
			wxGetApp().getErrorHandler()->handle(ErrorCode::MathExprError);
			return;
		}
		double minx = xmin->GetValue();
		double maxx = xmax->GetValue();
		double miny = ymin->GetValue();
		double maxy = ymax->GetValue();
		double step = this->step->GetValue();
		double speed = this->speed->GetValue();
		coord_point_t min = { minx, miny };
		coord_point_t max = { maxx, maxy };
		std::unique_ptr<GraphBuilder> graph = std::make_unique<GraphBuilder>(std::move(node), min, max, step);
		CalxVirtualPlaneDialog *dialog = new CalxVirtualPlaneDialog(
		    this, wxID_ANY, this->controller->getHandle(), wxSize(500, 500));

		this->controller->preview(dialog, this->translator->getTranslator(), std::move(graph),
		                          speed);
		dialog->ShowModal();
		delete dialog;
	}
}
