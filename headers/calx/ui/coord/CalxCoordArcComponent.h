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

#ifndef CALX_UI_CALX_COORD_ARC_CTRL_H_
#define CALX_UI_CALX_COORD_ARC_CTRL_H_

#include "calx/ui/CalxActionQueue.h"
#include "calx/ui/CalxApp.h"
#include "calx/ui/coord/CalxCoordComponent.h"
#include "calx/ui/coord/CalxCoordController.h"
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>

namespace CalX::UI {

	class CalxCoordArcComponentFactory : public CalxCoordComponentFactory {
	 public:
		CalxCoordComponent *newComponent(wxWindow *,
		                                 CalxCoordController *) override;
	};

	class CalxCoordArcComponent : public CalxCoordComponent {
	 public:
		CalxCoordArcComponent(wxWindow *, wxWindowID, CalxCoordController *);

	 private:
		void OnMoveClick(wxCommandEvent &);

		wxSpinCtrlDouble *xCoord;
		wxSpinCtrlDouble *yCoord;
		wxSpinCtrlDouble *cxCoord;
		wxSpinCtrlDouble *cyCoord;
		wxSpinCtrlDouble *speed;
		wxSpinCtrl *splitter;
		wxCheckBox *clockwise;
		wxCheckBox *relative;

		CalxCoordController *controller;
	};
}  // namespace CalX::UI

#endif
