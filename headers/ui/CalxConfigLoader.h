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

#ifndef CALX_UI_CALX_CONFIG_LOADER_H_
#define CALX_UI_CALX_CONFIG_LOADER_H_

#include "ui/CalxApp.h"
#include <vector>
#include <wx/dataview.h>
#include <wx/filesys.h>
#include <wx/textctrl.h>

namespace CalXUI {

	struct CalxConfigFile {
		std::string path;
		std::string name;
		std::string descr;
	};

	class CalxConfigLoader : public wxDialog {
	 public:
		CalxConfigLoader(wxWindow *, wxWindowID, std::string = "");
		void load();
		std::string getFileName();
		bool isExiting();

	 private:
		void OnLoadClick(wxCommandEvent &);
		void OnExitClick(wxCommandEvent &);
		void OnClose(wxCloseEvent &);

		bool exiting;
		std::string filename;
		wxFileSystem filesys;
		std::vector<CalxConfigFile> found;
		wxDataViewListCtrl *configList;
	};
}

#endif