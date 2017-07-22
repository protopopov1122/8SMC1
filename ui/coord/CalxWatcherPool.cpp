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

#include "ui/coord/CalxWatcherPool.h"
#include "ui/coord/CalxCoordPlaneWatcher.h"
#include <algorithm>

namespace CalXUI {

	CalxWatcherPool::CalxWatcherPool(wxWindow *win,
	                                 std::shared_ptr<CoordHandle> handle)
	    : parent(win), handle(handle) {}

	CalxWatcherPool::~CalxWatcherPool() {
		while (this->watchers.size() > 0) {
			const auto &w = this->watchers.at(0);
			w->Close(true);
		}
	}

	void CalxWatcherPool::bindWatcher(CalxCoordPlaneWatcher *w) {
		this->watchers.push_back(w);
	}

	void CalxWatcherPool::unbindWatcher(CalxCoordPlaneWatcher *w) {
		this->watchers.erase(
		    std::remove(this->watchers.begin(), this->watchers.end(), w),
		    this->watchers.end());
	}

	bool CalxWatcherPool::hasWatchers() {
		return !this->watchers.empty();
	}

	void CalxWatcherPool::updateWatchers() {
		for (const auto &w : this->watchers) {
			w->update();
		}
	}

	CalxCoordPlaneWatcher *CalxWatcherPool::newWatcher() {
		if (!this->handle->isMeasured()) {
			wxMessageBox(__("Plane need to be measured before preview"),
			             __("Warning"), wxICON_WARNING);
			return nullptr;
		}
		CalxCoordPlaneWatcherDialog *watcher = new CalxCoordPlaneWatcherDialog(
		    this->parent, wxID_ANY, this->handle, this);
		watcher->Show(true);
		return watcher->getWatcher();
	}
}
