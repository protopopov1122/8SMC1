
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

#include "calx/ui/coord/CalxCoordActions.h"
#include "calx/ui/CalxErrorHandler.h"
#include "calx/ctrl-lib/logger/Shortcuts.h"

namespace CalX::UI {

	CalxCoordActionGraphPreview::CalxCoordActionGraphPreview(
	    CalxVirtualPlaneDialog *dialog, std::shared_ptr<CoordTranslator> trans,
	    std::unique_ptr<GraphBuilder> builder, double speed)
	    : dialog(dialog),
	      translator(trans),
	      builder(std::move(builder)),
	      speed(speed) {
		this->state = std::make_shared<TaskState>();
		this->state->plane = nullptr;
		this->state->work = false;
	}

	ErrorCode CalxCoordActionGraphPreview::perform(SystemManager &sysman) {
		dialog->Enable(false);
		ErrorCode errcode = builder->floatBuild(sysman, dialog->getFloatPlane(),
		                                        *translator, speed, *state);
		wxGetApp().getErrorHandler()->handle(errcode);
		dialog->Refresh();
		dialog->Enable(true);
		return errcode;
	}

	void CalxCoordActionGraphPreview::stop() {
		this->state->stop();
	}
}  // namespace CalX::UI
