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

#ifndef CALX_CTRL_LIB_PLANE_VIRTUAL_COORD_PLANE_H_
#define CALX_CTRL_LIB_PLANE_VIRTUAL_COORD_PLANE_H_

#include "calx/ctrl-lib/plane/AbstractCoordPlane.h"

namespace CalX {

	class VirtualCoordPlane : public CoordPlane {
	 public:
		VirtualCoordPlane(motor_point_t, motor_rect_t);

		ErrorCode move(motor_point_t, float, bool) override;
		ErrorCode arc(motor_point_t, motor_point_t, int, float, bool,
		              float = 1.0f) override;
		ErrorCode calibrate(TrailerId) override;
		ErrorCode measure(TrailerId) override;
		motor_point_t getPosition() const override;
		motor_rect_t getSize() const override;
		bool isMeasured() const override;
		virtual std::unique_ptr<CoordPlane> clone(std::shared_ptr<CoordPlane>) = 0;
		CoordPlaneStatus getStatus() const override;
		ErrorCode open_session() override;
		ErrorCode close_session() override;
		bool isSessionOpened() const override;

		void use() override;
		void unuse() override;
		void stop() override;
		std::ostream &operator<<(std::ostream &) const override;

	 protected:
		virtual bool jump(motor_point_t, bool) = 0;

	 private:
		motor_point_t position;
		motor_rect_t size;
	};
}  // namespace CalX

#endif
