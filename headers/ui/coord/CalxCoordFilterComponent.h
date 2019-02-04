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

#ifndef CALX_UI_COORD_CALX_COORD_FILTER_CTRL_H_
#define CALX_UI_COORD_CALX_COORD_FILTER_CTRL_H_

#include "ui/CalxActionQueue.h"
#include "ui/CalxApp.h"
#include "ui/coord/CalxCoordComponent.h"
#include "ui/coord/CalxCoordController.h"
#include <cmath>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/collpane.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>

wxDECLARE_EVENT(wxEVT_FILTER_UPDATE_OFFSET, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_FILTER_UPDATE_SCALE, wxThreadEvent);

namespace CalX::UI {

	class CalxCoordFilterComponentFactory : public CalxCoordComponentFactory {
	 public:
		CalxCoordComponent *newComponent(wxWindow *,
		                                 CalxCoordController *) override;
	};

	class CalxCoordFilterComponent : public CalxCoordComponent,
	                                 public CalxCoordFilterListener {
	 public:
		CalxCoordFilterComponent(wxWindow *, wxWindowID, CalxCoordController *);

		void updateOffset(motor_point_t) override;
		void updateScale(motor_scale_t) override;

	 private:
		void OnUpdateOffset(wxThreadEvent &);
		void OnUpdateScale(wxThreadEvent &);
		void OnFiltersUpdate(wxCommandEvent &);
		void OnExit(wxCloseEvent &);

		wxCheckBox *logActions;
		wxCheckBox *logErrors;
		wxSpinCtrl *xOffset;
		wxSpinCtrl *yOffset;
		wxSpinCtrlDouble *xScale;
		wxSpinCtrlDouble *yScale;
		wxSpinCtrl *minx;
		wxSpinCtrl *miny;
		wxSpinCtrl *maxx;
		wxSpinCtrl *maxy;
		wxSpinCtrl *speed;

		CalxCoordController *controller;
	};
}  // namespace CalX::UI

#endif
