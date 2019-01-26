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

#ifndef CALX_UI_COORD_CALX_WATCHER_POOL_H_
#define CALX_UI_COORD_CALX_WATCHER_POOL_H_

#include "ui/CalxApp.h"
#include <vector>

namespace CalX::UI {

	class CalxCoordPlaneWatcher;  // Forward referencing

	class CalxWatcherPool {
	 public:
		CalxWatcherPool(wxWindow *, std::shared_ptr<CoordHandle>);
		virtual ~CalxWatcherPool();

		CalxCoordPlaneWatcher *newWatcher();
		bool hasWatchers();
		void updateWatchers();
		void bindWatcher(CalxCoordPlaneWatcher *);
		void unbindWatcher(CalxCoordPlaneWatcher *);

	 private:
		wxWindow *parent;
		std::shared_ptr<CoordHandle> handle;
		std::vector<CalxCoordPlaneWatcher *> watchers;
	};
}  // namespace CalX::UI

#endif
