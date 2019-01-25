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

#ifndef CALX_UI_CALX_COORD_GRAPH_CTRL_H_
#define CALX_UI_CALX_COORD_GRAPH_CTRL_H_

#include "ui/CalxActionQueue.h"
#include "ui/CalxApp.h"
#include "ui/coord/CalxCoordComponent.h"
#include "ui/coord/CalxCoordController.h"
#include "ui/coord/CalxCoordFilter.h"
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>

namespace CalXUI {

	class CalxCoordGraphComponentFactory : public CalxCoordComponentFactory {
		CalxCoordComponent *newComponent(wxWindow *,
		                                 CalxCoordController *) override;
	};

	class CalxCoordGraphComponent : public CalxCoordComponent {
	 public:
		CalxCoordGraphComponent(wxWindow *, wxWindowID, CalxCoordController *);

	 private:
		void OnClose(wxCloseEvent &);
		void OnBuildClick(wxCommandEvent &);
		void OnPreviewClick(wxCommandEvent &);

		wxTextCtrl *expr;
		wxSpinCtrlDouble *xmin;
		wxSpinCtrlDouble *xmax;
		wxSpinCtrlDouble *ymin;
		wxSpinCtrlDouble *ymax;
		wxSpinCtrlDouble *step;
		CalxCoordFilter *translator;
		std::shared_ptr<ComplexCoordTranslator> trans;
		wxSpinCtrlDouble *speed;

		CalxCoordController *controller;
	};
}  // namespace CalXUI

#endif
