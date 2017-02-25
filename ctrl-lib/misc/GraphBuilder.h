#ifndef _8SMC1_CTRL_LIB_GRAPH_BUILDER_H_
#define _8SMC1_CTRL_LIB_GRAPH_BUILDER_H_

#include "ctrl-lib/graph/FunctionEngine.h"
#include "ctrl-lib/plane/CoordPlane.h"

/* Graph builder contains routine functions to build mathematical
   function graphs to coordinate planes. It stores function,
   minimal and maximal x&y and x calculation step and takes plane,
   coordinate translator and speed to build. */

namespace _8SMC1 {
	class SystemManager;
	
	class GraphBuilder {
		public:
			GraphBuilder(Node*, coord_point_t, coord_point_t, long double);
			virtual ~GraphBuilder();
			
			Node *getFunction();
			coord_point_t getMinimum();
			coord_point_t getMaximum();
			long double getStep();
			
			ErrorCode build(SystemManager*, CoordPlane*, CoordTranslator*, float);
		private:
			Node *node;
			coord_point_t min;
			coord_point_t max;
			long double step;
	};
}

#endif