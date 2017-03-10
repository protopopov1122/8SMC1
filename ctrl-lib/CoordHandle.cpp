#include "CoordHandle.h"

namespace CalX {
	
		CoordHandle::CoordHandle(size_t id, CoordController *root)
			: CoordPlaneStack::CoordPlaneStack(root) {
			this->id = id;
			this->root = root;
		}
		
		CoordHandle::~CoordHandle() {
			
		}
		
		size_t CoordHandle::getID() {
				return this->id;
		}
		
		CoordController *CoordHandle::getController() {
			return this->root;
		}
		
		void CoordHandle::addEventListener(CoordEventListener *l) {
			this->root->addEventListener(l);
		}
		
		void CoordHandle::removeEventListener(CoordEventListener *l) {
			this->root->removeEventListener(l);
		}
}