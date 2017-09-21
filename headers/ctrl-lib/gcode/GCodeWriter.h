/*
        Copyright (c) 2017 Jevgenijs Protopopovs

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

#ifndef CALX_CTRL_LIB_GCODE_WRITER_H_
#define CALX_CTRL_LIB_GCODE_WRITER_H_

#include "ctrl-lib/CtrlCore.h"
#include "ctrl-lib/plane/CoordPlane.h"
#include "ctrl-lib/translator/CoordTranslator.h"
#include <iostream>

namespace CalX {

	class GCodeWriter : public VirtualCoordPlane {
	 public:
		GCodeWriter(motor_point_t, motor_rect_t, std::shared_ptr<CoordTranslator>,
		            std::ostream &);
		void close();
		virtual std::unique_ptr<CoordPlane> clone(std::shared_ptr<CoordPlane>);

	 protected:
		virtual void jump(motor_point_t, bool);

	 private:
		std::shared_ptr<CoordTranslator> trans;
		std::ostream &out;
	};
}  // namespace CalX

#endif
