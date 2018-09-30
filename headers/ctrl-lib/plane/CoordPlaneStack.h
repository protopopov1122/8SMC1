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

#ifndef CALX_CTRL_LIB_PLANE_COORD_PLANE_STACK_H_
#define CALX_CTRL_LIB_PLANE_COORD_PLANE_STACK_H_

#include "ctrl-lib/plane/AbstractCoordPlane.h"

namespace CalX {

	// Coordinate plane abstraction stacking
	class CoordPlaneStack : public CoordPlane {
	 public:
		CoordPlaneStack(std::shared_ptr<CoordPlane>);

		virtual std::shared_ptr<CoordPlane> peekPlane() const;
		virtual bool popPlane();
		virtual void pushPlane(std::shared_ptr<CoordPlane>);
		virtual std::shared_ptr<CoordPlane> getBase() const;

		ErrorCode move(motor_point_t, float, bool) override;
		ErrorCode arc(motor_point_t, motor_point_t, int, float, bool,
		                      float = 1.0f) override;
		ErrorCode calibrate(TrailerId) override;
		ErrorCode measure(TrailerId) override;
		motor_point_t getPosition() const override;
		motor_rect_t getSize() const override;
		bool isMeasured() const override;
		void use() override;
		void unuse() override;
		void stop() override;
		std::ostream &operator<<(std::ostream &) const override;
		std::unique_ptr<CoordPlane> clone(std::shared_ptr<CoordPlane>) override;
		CoordPlaneStatus getStatus() const override;
		ErrorCode open_session() override;
		ErrorCode close_session() override;
		bool isSessionOpened() const override;

	 private:
		std::vector<std::shared_ptr<CoordPlane>> stack;
	};
}  // namespace CalX

#endif
