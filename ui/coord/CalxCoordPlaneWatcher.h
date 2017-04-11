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


#ifndef CALX_UI_COORD_CALX_COORD_PLANE_WATCHER_H_
#define CALX_UI_COORD_CALX_COORD_PLANE_WATCHER_H_

#include "CalxApp.h"
#include <wx/stattext.h>

namespace CalXUI {
	
	class CalxCoordPlaneWatcher; // Forward referencing
	
	class CalxCoordPlaneWatcherTimer : public wxTimer {
		public:
			CalxCoordPlaneWatcherTimer(CalxCoordPlaneWatcher*, CoordHandle*);
			virtual void Notify();
		private:
			CalxCoordPlaneWatcher *watcher;
			CoordHandle *handle;
	};
	
	class CalxCoordPlaneWatcherEvent : public CoordEventListener {
		public:
			CalxCoordPlaneWatcherEvent(CalxCoordPlaneWatcher*);
			virtual ~CalxCoordPlaneWatcherEvent();
			virtual void moved(CoordMoveEvent&);
		private:
			CalxCoordPlaneWatcher *watcher;
	};
	
	class CalxCoordPlaneWatcher : public wxWindow {
		public:
			CalxCoordPlaneWatcher(wxWindow*, wxWindowID, wxSize, CoordHandle*);
			void clear();
			void add(motor_point_t, bool);
		private:
			void render(wxDC&);
			void OnExit(wxCloseEvent&);
			void OnPaintEvent(wxPaintEvent&);
			void OnResizeEvent(wxSizeEvent&);
			CoordHandle *handle;
			CalxCoordPlaneWatcherTimer *timer;
			std::vector<std::pair<motor_point_t, bool>> history;
			CalxCoordPlaneWatcherEvent *listener;
	};
	
	class CalxCoordPlaneWatcherDialog : public wxDialog {
		public:
			CalxCoordPlaneWatcherDialog(wxWindow*, wxWindowID, CoordHandle*);
		private:
			void OnOkClick(wxCommandEvent&);
			void OnClearClick(wxCommandEvent&);
			void OnMouseMove(wxMouseEvent&);
			CalxCoordPlaneWatcher *watcher;
			CoordHandle *handle;
			wxStaticText *mouseCoords;
	};
}

#endif