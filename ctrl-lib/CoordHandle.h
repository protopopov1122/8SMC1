#ifndef CALX_CTRL_LIB_COORD_HANDLE_H_
#define CALX_CTRL_LIB_COORD_HANDLE_H_

#include <cinttypes>
#include "plane/CoordPlane.h"

namespace CalX {

	class CoordHandle : public CoordPlaneStack {
		public:
			CoordHandle(size_t, CoordController*);
			virtual ~CoordHandle();
			
			size_t getID();
			CoordController *getController();
			
			void addEventListener(CoordEventListener*);
			void removeEventListener(CoordEventListener*);
		private:
			size_t id;
			CoordController *root;
	};
}

#endif