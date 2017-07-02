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


#ifndef CALX_CTRL_LIB_PLANE_COORD_PLANE_MAP_H_
#define CALX_CTRL_LIB_PLANE_COORD_PLANE_MAP_H_


#include "ctrl-lib/plane/AbstractCoordPlane.h"

namespace CalX {

	class CoordPlaneMap : public CoordPlane {
		public:
			CoordPlaneMap(motor_point_t, motor_scale_t, float, CoordPlane*);
			virtual ~CoordPlaneMap();
			CoordPlane *getBase();
			motor_point_t getOffset();
			motor_scale_t getScale();
			float getSpeedScale();
			void setOffset(motor_point_t);
			void setScale(motor_scale_t);
			void setSpeedScale(float);

			virtual ErrorCode move(motor_point_t, float, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, bool, float = 1.0f);
			virtual ErrorCode calibrate(TrailerId);
			virtual ErrorCode measure(TrailerId);
			virtual motor_point_t getPosition();
			virtual motor_rect_t getSize();
			virtual bool isMeasured();
			virtual void use();
			virtual void unuse();
			virtual void stop();
			virtual void dump(std::ostream&);
			virtual CoordPlane *clone(CoordPlane*);
			virtual CoordPlaneStatus getStatus();
			virtual ErrorCode open_session();
			virtual ErrorCode close_session();
		private:
			CoordPlane* plane;
			motor_point_t offset;
			motor_scale_t scale;
			float speedScale;
	};
}


#endif
