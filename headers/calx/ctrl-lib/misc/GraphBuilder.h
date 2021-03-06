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

#ifndef CALX_CTRL_LIB_GRAPH_BUILDER_H_
#define CALX_CTRL_LIB_GRAPH_BUILDER_H_

#include "calx/ctrl-lib/graph/MathEngine.h"
#include "calx/ctrl-lib/plane/CoordPlane.h"
#include "calx/ctrl-lib/translator/CoordTranslator.h"

/* Graph builder contains routine functions to build mathematical
   function graphs to coordinate planes. It stores function,
   minimal and maximal x&y and x calculation step and takes plane,
   coordinate translator and speed to build. */

namespace CalX {
	class SystemManager;

	class GraphBuilder {
	 public:
		GraphBuilder(std::unique_ptr<Node>, coord_point_t, coord_point_t, double);

		coord_point_t getMinimum() const;
		coord_point_t getMaximum() const;
		double getStep() const;

		ErrorCode build(SystemManager &, std::shared_ptr<CoordPlane>,
		                CoordTranslator &, float, TaskState &) const;
		ErrorCode floatBuild(SystemManager &, std::shared_ptr<FloatCoordPlane>,
		                     CoordTranslator &, float, TaskState &) const;

		void dump(std::ostream &) const;
		friend std::ostream &operator<<(std::ostream &, const GraphBuilder &);

	 private:
		std::unique_ptr<Node> node;
		coord_point_t min;
		coord_point_t max;
		double step;
	};
}  // namespace CalX

#endif
