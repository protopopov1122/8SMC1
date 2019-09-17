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

#ifndef CALX_UI_CALX_COORD_POSITION_CTRL_H_
#define CALX_UI_CALX_COORD_POSITION_CTRL_H_

#include "calx/ui/CalxApp.h"
#include "calx/ui/coord/CalxCoordComponent.h"
#include "calx/ui/coord/CalxCoordController.h"
#include <wx/spinctrl.h>

namespace CalX::UI {

	class CalxCoordPositionComponentFactory : public CalxCoordComponentFactory {
		virtual CalxCoordComponent *newComponent(wxWindow *, CalxCoordController *);
	};

	class CalxCoordPositionComponent : public CalxCoordComponent {
	 public:
		CalxCoordPositionComponent(wxWindow *, wxWindowID, CalxCoordController *);

	 private:
		void OnMoveClick(wxCommandEvent &);
		void OnConfigureClick(wxCommandEvent &);
		void OnSaveClick(wxCommandEvent &);

		wxSpinCtrlDouble *xPos;
		wxSpinCtrlDouble *yPos;
		wxSpinCtrlDouble *speed;

		CalxCoordController *controller;
	};
}  // namespace CalX::UI

#endif
