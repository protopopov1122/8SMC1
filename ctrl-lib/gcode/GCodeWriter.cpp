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

#include "ctrl-lib/gcode/GCodeWriter.h"

namespace CalX {

	GCodeWriter::GCodeWriter(motor_point_t pos, motor_rect_t size,
	                         CoordTranslator *trans, std::ostream *out)
	    : VirtualCoordPlane::VirtualCoordPlane(pos, size) {
		this->trans = trans;
		this->out = out;
		*out << "%" << std::endl;
	}

	GCodeWriter::~GCodeWriter() {
		*out << "%" << std::endl;
	}

	CoordPlane *GCodeWriter::clone(CoordPlane *base) {
		GCodeWriter *writer = new GCodeWriter(base->getPosition(), base->getSize(),
		                                      this->trans, this->out);
		return writer;
	}

	void GCodeWriter::jump(motor_point_t pnt, bool move) {
		coord_point_t point = this->trans->get(pnt);
		*out << (move ? "G01" : "G00") << " X" << point.x << " Y" << point.y
		     << std::endl;
	}
}
