/*
        Copyright (c) 2017-2018 Jevgenijs Protopopovs

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

#ifndef CALX_CTRL_LIB_PLANE_ABSTRACT_COORD_PLANE_H_
#define CALX_CTRL_LIB_PLANE_ABSTRACT_COORD_PLANE_H_

#include "ctrl-lib/Resource.h"
#include "ctrl-lib/EventListener.h"
#include "ctrl-lib/conf/Dictionary.h"
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

/* Coordinate plane represents an abstraction of two moving axis
   (typically motors) and some cutting instrument (optional).
   It offers different movement/cutting modes based on simple motor
   and instrument abstractions.
   Coordinate plane has several elementar actions:
     * linear movement - synchrone and asynchrone
     * arc movement
     * relative linear and arc movement - all coordinates are calculated
     using current position and offset
     * calibration - both motors are moved to the start or end of axis
     * measurement - plane performs double calibration to determine available
     axis length
    All actions should be performed during session. To open/close session use
    appropriate methods
*/

namespace CalX {
	enum class CoordPlaneStatus {
		Idle, /* None of devices (motors/instruments) are enabled/moving */
		Jump, /* Cutting instrument disabled, movement performed asynchroniously to
		         achieve maximal speed */
		Move  /* Cutting instrument may be enabled (depends on several different
		         settings), motors move  synchronously */
	};

	/* Abstract interface. By default it uses motor-specific units to operate,
	   See FloatCoordPlane that can use any-scale units */
	class CoordPlane : public UsableResource, public SessionableResource {
	 public:
		virtual ~CoordPlane() = default;
		virtual ErrorCode move(
		    motor_point_t, /* Destination point. Measured in motor-specific steps */
		    float,         /* Speed. Measured in motor-specific units */
		    bool           /* Move/Jump mode */
		    ) = 0;
		virtual ErrorCode relativeMove(motor_point_t, float,
		                               bool); /* The same as above, but
		                                         destination point is calculated
		                                         relatively to current position */
		virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, bool,
		                      float = 1.0f) = 0;
		virtual ErrorCode relativeArc(motor_point_t, motor_point_t, int, float,
		                              bool, float = 1.0f);
		virtual ErrorCode calibrate(TrailerId) = 0;
		virtual ErrorCode measure(TrailerId) = 0;
		virtual motor_point_t getPosition() const = 0;
		virtual motor_rect_t getSize() const = 0;
		virtual bool isMeasured() const = 0;
		virtual bool isSessionOpened() const = 0;
		virtual void stop() = 0;
		virtual std::ostream &operator<<(std::ostream &) const = 0;
		virtual std::unique_ptr<CoordPlane> clone(std::shared_ptr<CoordPlane>) = 0;
		virtual CoordPlaneStatus getStatus() const = 0;
		ErrorCode open_session() override;
		ErrorCode close_session() override;
	};

	struct TaskState {
		std::shared_ptr<CoordPlane> plane;
		bool work;

		void stop();
	};
}  // namespace CalX

#endif
