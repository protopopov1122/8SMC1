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