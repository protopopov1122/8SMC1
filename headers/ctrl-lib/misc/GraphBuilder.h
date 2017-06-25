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


#ifndef CALX_CTRL_LIB_GRAPH_BUILDER_H_
#define CALX_CTRL_LIB_GRAPH_BUILDER_H_

#include "ctrl-lib/graph/FunctionEngine.h"
#include "ctrl-lib/plane/CoordPlane.h"

/* Graph builder contains routine functions to build mathematical
   function graphs to coordinate planes. It stores function,
   minimal and maximal x&y and x calculation step and takes plane,
   coordinate translator and speed to build. */

namespace CalX {
	class SystemManager;
	
	class GraphBuilder {
		public:
			GraphBuilder(Node*, coord_point_t, coord_point_t, double);
			virtual ~GraphBuilder();
			
			Node *getFunction();
			coord_point_t getMinimum();
			coord_point_t getMaximum();
			double getStep();
			
			ErrorCode build(SystemManager*, CoordPlane*, CoordTranslator*, float, TaskState*);
			ErrorCode floatBuild(SystemManager*, FloatCoordPlane*, CoordTranslator*, float, TaskState*);
		private:
			Node *node;
			coord_point_t min;
			coord_point_t max;
			double step;
	};
}

#endif