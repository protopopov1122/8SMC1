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

#ifndef _CALX_UI_CALX_COM_SELECT_DIALOG_H_
#define _CALX_UI_CALX_COM_SELECT_DIALOG_H_

#include "ui/CalxApp.h"
#include <wx/choice.h>
#include <wx/spinctrl.h>

namespace CalXUI {

	class CalxCOMSelectDialog : public wxDialog {
	 public:
		CalxCOMSelectDialog(wxWindow *, wxWindowID);
		int16_t getPort();
		int32_t getSpeed();
		SerialPortParity getParity();

	 private:
		void OnOkClick(wxCommandEvent &);
		void OnCancelClick(wxCommandEvent &);

		wxSpinCtrl *portSpin;
		wxSpinCtrl *speedSpin;
		wxChoice *parityCh;
		int16_t port;
		int32_t speed;
		SerialPortParity parity;
	};
}

#endif
