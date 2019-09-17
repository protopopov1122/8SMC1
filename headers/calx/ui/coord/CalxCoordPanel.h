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

#ifndef CALX_UI_CALX_COORD_PANEL_H_
#define CALX_UI_CALX_COORD_PANEL_H_

#include "calx/ui/CalxApp.h"
#include "calx/ui/CalxPanelPane.h"
#include "calx/ui/coord/CalxPlaneList.h"
#include "calx/ui/coord/CalxCoordComponent.h"
#include "calx/ui/coord/CalxCoordPane.h"
#include <cinttypes>
#include <vector>
#include <wx/listbox.h>

namespace CalX::UI {

	wxDECLARE_EVENT(wxEVT_COORD_PANEL_UPDATE, wxThreadEvent);

	struct CalxCoordComponentFactoryHandle {
	 public:
		CalxCoordComponentFactoryHandle(const std::string &title,
		                                CalxCoordComponentFactory *factory,
		                                bool hidden)
		    : title(title), factory(factory), hidden(hidden) {}
		std::string title;
		CalxCoordComponentFactory *factory;
		bool hidden;
	};

	class CalxCoordPanel : public CalxPanelPane, public CalxPlaneList {
	 public:
		CalxCoordPanel(wxWindow *, wxWindowID, std::size_t);

		std::size_t getPlaneCount() override;
		CalxCoordPane *getCoordCtrl(std::size_t sz);
		void updateList(std::shared_ptr<CoordHandle>, bool *) override;
		CalxPlaneHandle *getPlaneHandle(std::size_t) override;
		void updateUI() override;
		void shutdown() override;
		bool isBusy() override;
		void stop() override;
		std::size_t getLayoutColumnCount();
		bool addComponentFactory(const std::string &, CalxCoordComponentFactory *,
		                         std::size_t, bool = true);

	 private:
		void addPlane(std::shared_ptr<CoordHandle>);
		void removePlane(std::size_t);

		void OnExit(wxCloseEvent &);
		void OnAddButtonClick(wxCommandEvent &);
		void OnRemoveButtonClick(wxCommandEvent &);
		void OnListClick(wxCommandEvent &);
		void OnCoordPlaneAdd(wxThreadEvent &);

		wxListBox *coordList;
		std::vector<CalxCoordPane *> coords;
		std::vector<std::vector<CalxCoordComponentFactoryHandle>> layout;
		wxPanel *mainPanel;
		int nextId;
	};
}  // namespace CalX::UI

#endif
