/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CALX_UI_MATH_CALX_MATH_OBJECT_H_
#define CALX_UI_MATH_CALX_MATH_OBJECT_H_

#include "calx/ui/CalxApp.h"
#include "calx/ctrl-lib/graph/Formula.h"

namespace CalX::UI {

	class CalxMathObject : public wxPanel {
	 public:
		using wxPanel::wxPanel;
		virtual const std::string &getTitle() const = 0;
	};

	class CalxMathFormulaPanel : public CalxMathObject {
	 public:
		CalxMathFormulaPanel(wxWindow *, wxWindowID, const std::string &,
		                     const std::string &,
		                     const std::map<std::string, std::string> &);
		const std::string &getTitle() const override;

	 private:
		void OnCalculateClick(wxCommandEvent &);

		std::string title;
		std::string formulaText;
		std::unique_ptr<MathFormula> formula;

		wxStaticText *resultText;
		std::map<std::string, double> variableValues;
	};
}  // namespace CalX::UI

#endif