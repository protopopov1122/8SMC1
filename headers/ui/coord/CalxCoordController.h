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

#ifndef CALX_UI_COORD_CALX_COORD_CONTROLLER_H_
#define CALX_UI_COORD_CALX_COORD_CONTROLLER_H_

#include "ui/CalxActionQueue.h"
#include "ui/CalxApp.h"
#include "ui/coord/CalxVirtualPlane.h"

namespace CalXUI {

	class CalxCoordFilterListener {
	 public:
		virtual ~CalxCoordFilterListener() = default;
		virtual void updateOffset(motor_point_t) = 0;
		virtual void updateScale(motor_scale_t) = 0;
	};

	class CalxCoordController {
	 public:
		CalxCoordController(std::shared_ptr<CoordHandle>, CalxActionQueue *);
		virtual ~CalxCoordController() = default;

		std::shared_ptr<CoordHandle> getHandle();

		motor_point_t getOffset();
		motor_scale_t getScale();
		void setOffset(motor_point_t);
		void setScale(motor_scale_t);
		void addFilterListener(CalxCoordFilterListener *);
		void removeFilterListener(CalxCoordFilterListener *);

		bool isLoggingActions();
		void setLoggingActions(bool);
		bool isLoggingErrors();
		void setLoggingErrors(bool);

		motor_point_t getValidateMinimum();
		void setValidateMinimum(motor_point_t);
		motor_point_t getValidateMaximum();
		void setValidateMaximum(motor_point_t);
		float getValidateMaxSpeed();
		void setValidateMaxSpeed(float);

		CalxActionResult move(coord_point_t, double, bool, bool);
		CalxActionResult arc(coord_point_t, coord_point_t, int, double, bool, bool);
		CalxActionResult calibrate(TrailerId);
		CalxActionResult measure(TrailerId);
		CalxActionResult move(coord_point_t, double);
		CalxActionResult configure(coord_point_t, double);
		CalxActionResult build(std::shared_ptr<CoordTranslator>,
		                       std::unique_ptr<GraphBuilder>, double);
		CalxActionResult preview(CalxVirtualPlaneDialog *,
		                         std::shared_ptr<CoordTranslator>,
		                         std::unique_ptr<GraphBuilder>, double);

	 private:
		std::shared_ptr<CoordHandle> handle;
		CalxActionQueue *queue;

		// Filters
		std::vector<CalxCoordFilterListener *> listeners;
		std::shared_ptr<CoordPlaneLog> log;
		std::shared_ptr<CoordPlaneMap> map;
		std::shared_ptr<CoordPlaneValidator> validator;
		std::shared_ptr<CoordPlaneMap> unit_map;
	};
}  // namespace CalXUI

#endif
