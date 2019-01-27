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

#ifndef CALX_UI_COORD_CALX_COORD_ACTIONS_H_
#define CALX_UI_COORD_CALX_COORD_ACTIONS_H_

#include "ctrl-lib/actions/PlaneActions.h"
#include "ui/CalxActionQueue.h"
#include "ui/CalxApp.h"
#include "ui/coord/CalxCoordController.h"
#include "ui/coord/CalxVirtualPlane.h"

namespace CalX::UI {

	class CalxCoordActionGraphPreview : public CalxAction {
	 public:
		CalxCoordActionGraphPreview(CalxVirtualPlaneDialog *,
		                            std::shared_ptr<CoordTranslator>,
		                            std::unique_ptr<GraphBuilder>, double);

		ErrorCode perform(SystemManager &) override;
		void stop() override;

	 private:
		CalxVirtualPlaneDialog *dialog;
		std::shared_ptr<CoordTranslator> translator;
		std::unique_ptr<GraphBuilder> builder;
		double speed;
		std::shared_ptr<TaskState> state;
	};
}  // namespace CalX::UI

#endif
