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

#ifndef CALX_UI_COORD_CALX_COORD_PLANE_WATCHER_H_
#define CALX_UI_COORD_CALX_COORD_PLANE_WATCHER_H_

#include "ui/CalxApp.h"
#include "ui/coord/CalxWatcherPool.h"
#include <wx/bitmap.h>
#include <wx/stattext.h>

namespace CalX::UI {

	class CalxCoordPlaneWatcher;  // Forward referencing

	wxDECLARE_EVENT(wxEVT_WATCHER_UPDATE, wxThreadEvent);
	wxDECLARE_EVENT(wxEVT_WATCHER_APPEND_POINT, wxThreadEvent);

	class CalxCoordPlaneWatcherTimer : public wxTimer {
	 public:
		CalxCoordPlaneWatcherTimer(CalxCoordPlaneWatcher *,
		                           std::shared_ptr<CoordHandle>);
		void Notify() override;

	 private:
		CalxCoordPlaneWatcher *watcher;
		std::shared_ptr<CoordHandle> handle;
	};

	class CalxCoordPlaneWatcherRepaintTimer : public wxTimer {
	 public:
		CalxCoordPlaneWatcherRepaintTimer(CalxCoordPlaneWatcher *);
		void Notify() override;

	 private:
		CalxCoordPlaneWatcher *watcher;
	};

	class CalxCoordPlaneWatcherEvent : public CoordEventListener {
	 public:
		CalxCoordPlaneWatcherEvent(CalxCoordPlaneWatcher *,
		                           std::shared_ptr<CoordHandle>);
		virtual ~CalxCoordPlaneWatcherEvent();
		void moved(CoordMoveEvent &) override;

	 private:
		CalxCoordPlaneWatcher *watcher;
		std::shared_ptr<CoordHandle> handle;
	};

	class CalxCoordPlaneWatcher : public wxWindow {
	 public:
		CalxCoordPlaneWatcher(wxWindow *, wxWindowID, wxSize,
		                      std::shared_ptr<CoordHandle>, CalxWatcherPool *);
		void clear();
		bool isRendering();
		bool hasUpdates();
		std::shared_ptr<CoordHandle> getHandle();
		void update();

	 private:
		void add(motor_point_t, bool);
		void render(wxDC &);
		void renderBitmap();
		void OnExit(wxCloseEvent &);
		void OnPaintEvent(wxPaintEvent &);
		void OnResizeEvent(wxSizeEvent &);
		void OnAppendEvent(wxThreadEvent &);
		void OnUpdateEvent(wxThreadEvent &);

		std::shared_ptr<CoordHandle> handle;
		CalxWatcherPool *pool;
		CalxCoordPlaneWatcherTimer *timer;
		CalxCoordPlaneWatcherRepaintTimer *repaint_timer;
		std::vector<std::pair<motor_point_t, bool>> history;
		std::shared_ptr<CalxCoordPlaneWatcherEvent> listener;
		bool rendering;
		bool has_updates;

		wxBitmap bitmap;

		wxColour pointer_colour;
		wxColour jump_colour;
		wxColour move_colour;
	};

	class CalxCoordPlaneWatcherDialog : public wxDialog {
	 public:
		CalxCoordPlaneWatcherDialog(wxWindow *, wxWindowID,
		                            std::shared_ptr<CoordHandle>,
		                            CalxWatcherPool *);
		CalxCoordPlaneWatcher *getWatcher();

	 private:
		void OnOkClick(wxCommandEvent &);
		void OnClearClick(wxCommandEvent &);
		void OnMouseMove(wxMouseEvent &);

		CalxCoordPlaneWatcher *watcher;
		CalxWatcherPool *pool;
		std::shared_ptr<CoordHandle> handle;
		wxStaticText *mouseCoords;
	};
}  // namespace CalX::UI

#endif
