/*
        Copyright (c) 2017-2018 Jevgenijs Protopopovs

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

namespace CalXUI {

	class CalxCoordPlaneWatcher;  // Forward referencing

	wxDECLARE_EVENT(wxEVT_WATCHER_APPEND_POINT, wxThreadEvent);

	class CalxCoordPlaneWatcherTimer : public wxTimer {
	 public:
		CalxCoordPlaneWatcherTimer(CalxCoordPlaneWatcher *,
		                           std::shared_ptr<CoordHandle>);
		virtual void Notify();

	 private:
		CalxCoordPlaneWatcher *watcher;
		std::shared_ptr<CoordHandle> handle;
	};

	class CalxCoordPlaneWatcherRepaintTimer : public wxTimer {
	 public:
		CalxCoordPlaneWatcherRepaintTimer(CalxCoordPlaneWatcher *);
		virtual void Notify();

	 private:
		CalxCoordPlaneWatcher *watcher;
	};

	class CalxCoordPlaneWatcherEvent : public CoordEventListener {
	 public:
		CalxCoordPlaneWatcherEvent(CalxCoordPlaneWatcher *,
		                           std::shared_ptr<CoordHandle>);
		virtual ~CalxCoordPlaneWatcherEvent();
		virtual void moved(CoordMoveEvent &);

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
}  // namespace CalXUI

#endif
