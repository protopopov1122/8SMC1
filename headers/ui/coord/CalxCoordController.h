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

#ifndef CALX_UI_COORD_CALX_COORD_CONTROLLER_H_
#define CALX_UI_COORD_CALX_COORD_CONTROLLER_H_

#include "ui/CalxApp.h"
#include "ui/coord/CalxFilterController.h"
#include "ui/CalxActionQueue.h"
#include "ui/coord/CalxVirtualPlane.h"

namespace CalXUI {
	
	class CalxCoordController {
		public:
			CalxCoordController(CoordHandle*, CalxFilterController*, CalxActionQueue*);
			virtual ~CalxCoordController();
			
			CoordHandle *getHandle();
			CoordPlaneLog *getLogFilter();
			CoordPlaneMap *getMapFilter();
			CoordPlaneValidator *getValidateFilter();
			CoordPlaneMap *getUnitMapFilter();

			void move(coord_point_t, double, bool, bool, bool* = nullptr);
			void arc(coord_point_t, coord_point_t, int, double, bool, bool, bool* = nullptr);
			void calibrate(TrailerId, bool* = nullptr);
			void measure(TrailerId, bool* = nullptr);
			void move(coord_point_t, double, bool* = nullptr);
			void configure(coord_point_t, double, bool* = nullptr);
			void build(CoordTranslator*, GraphBuilder*, double, bool* = nullptr);
			void preview(CalxVirtualPlaneDialog*, CoordTranslator*, GraphBuilder*, double, bool* = nullptr);
		private:
			CoordHandle *handle;
			CalxFilterController *filters;
			CalxActionQueue *queue;
			
			// Filters
			CoordPlaneLog *log;
			CoordPlaneMap *map;
			CoordPlaneValidator *validator;
			CoordPlaneMap *unit_map;
	};
}

#endif