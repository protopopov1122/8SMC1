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

#ifndef CALX_UI_CALX_COORD_PANEL_H_
#define CALX_UI_CALX_COORD_PANEL_H_

#include <cinttypes>
#include <vector>
#include "ui/CalxApp.h"
#include <wx/listbox.h>
#include "ui/CalxPanelPane.h"
#include "ui/coord/CalxCoordPane.h"
#include "ui/coord/CalxCoordPlaneWatcher.h"

namespace CalXUI {

  wxDECLARE_EVENT(wxEVT_COORD_PANEL_UPDATE, wxThreadEvent);

  class CalxCoordPanel : public CalxPanelPane {
   public:
	CalxCoordPanel(wxWindow *, wxWindowID);

	size_t getCoordCount();
	CalxCoordPane *getCoordCtrl(size_t sz);
	void updateList(CoordHandle *, bool *);
	virtual void updateUI();
	virtual void shutdown();
	virtual bool isBusy();

   private:
	void addPlane(CoordHandle *);
	void removePlane(size_t);

	void OnExit(wxCloseEvent &);
	void OnAddButtonClick(wxCommandEvent &);
	void OnRemoveButtonClick(wxCommandEvent &);
	void OnListClick(wxCommandEvent &);
	void OnCoordPlaneAdd(wxThreadEvent &);

	wxListBox *coordList;
	std::vector<CalxCoordPane *> coords;
	wxPanel *mainPanel;
	int nextId;
  };
}

#endif
