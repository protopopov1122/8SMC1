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

#include "calx/ui/math/CalxMathObject.h"
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <limits>

namespace CalX::UI {

	CalxMathFormulaPanel::CalxMathFormulaPanel(
	    wxWindow *parent, wxWindowID id, const std::string &title,
	    const std::string &formula,
	    const std::map<std::string, std::string> &variables)
	    : CalxMathObject::CalxMathObject(parent, id),
	      title(title),
	      formulaText(formula),
	      formula(nullptr) {
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		this->SetSizer(sizer);

		this->resultText = new wxStaticText(this, wxID_ANY, formula);
		sizer->Add(this->resultText);

		wxPanel *parameterPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(parameterPanel);
		wxFlexGridSizer *parameterSizer = new wxFlexGridSizer(2);
		parameterPanel->SetSizer(parameterSizer);

		std::vector<std::string> variableKeys;
		for (auto it = variables.begin(); it != variables.end(); ++it) {
			variableKeys.push_back(it->first);

			wxStaticText *label = new wxStaticText(
			    parameterPanel, wxID_ANY, it->second + " (" + it->first + ")");
			parameterSizer->Add(label, 1,
			                    wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
			wxSpinCtrlDouble *input = new wxSpinCtrlDouble(
			    parameterPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
			    wxDefaultSize, wxSP_ARROW_KEYS, std::numeric_limits<double>::min(),
			    std::numeric_limits<double>::max(), 0, 0.000001);
			parameterSizer->Add(input);
			this->variableValues[it->first] = 0;
			std::string variableName = it->first;
			input->Bind(wxEVT_SPINCTRLDOUBLE,
			            [this, variableName](wxSpinDoubleEvent &evt) {
				            this->variableValues[variableName] = evt.GetValue();
			            });
		}
		wxButton *calculateButton = new wxButton(this, wxID_ANY, __("Calculate"));
		sizer->Add(calculateButton);
		calculateButton->Bind(wxEVT_BUTTON, &CalxMathFormulaPanel::OnCalculateClick,
		                      this);
		this->Layout();

		this->formula = MathFormula::build(formula, variableKeys);
	}

	const std::string &CalxMathFormulaPanel::getTitle() const {
		return this->title;
	}

	void CalxMathFormulaPanel::OnCalculateClick(wxCommandEvent &evt) {
		engine_value_t value = this->formula->eval(
		    wxGetApp().getSystemManager().getMathEngine(), this->variableValues);
		if (value.err == MathError::MNoError) {
			this->resultText->SetLabel(this->formulaText + " = " +
			                           std::to_string(value.value));
		} else {
			this->resultText->SetLabel(this->formulaText);
		}
		this->Layout();
	}
}  // namespace CalX::UI