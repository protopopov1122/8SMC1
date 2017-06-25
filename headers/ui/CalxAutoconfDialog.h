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


#ifndef CALX_UI_CALX_AUTOCONF_DIALOG_H_
#define CALX_UI_CALX_AUTOCONF_DIALOG_H_

#include <iostream>
#include "ui/CalxApp.h"
#include <wx/stattext.h>
#include "ctrl-lib/RequestResolver.h"


namespace CalXUI {

	wxDECLARE_EVENT(wxEVT_AUTOCONF_RESOLVING, wxThreadEvent);
	wxDECLARE_EVENT(wxEVT_AUTOCONF_FAILED, wxThreadEvent);
	
	struct CalxAutoconfInfo {
		std::string code;
		size_t step;
		size_t full;
	};

	class CalxAutoconfDialog : public wxDialog, public RequestResolveWatcher {
		public:
			CalxAutoconfDialog(wxWindow*, wxWindowID);
			virtual void resolving(std::string, size_t, size_t);
			virtual void failed(std::string, size_t, size_t);
		private:
			void OnResolvingEvent(wxThreadEvent&);
			void OnFailedEvent(wxThreadEvent&);
			void OnExit(wxCloseEvent&);
			wxStaticText *text;
	};
}

#endif
