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
	                         std::shared_ptr<CoordTranslator> trans,
	                         std::ostream &os)
	    : VirtualCoordPlane::VirtualCoordPlane(pos, size), out(os) {
		this->trans = trans;
		out << "%" << std::endl;
	}

	void GCodeWriter::close() {
		out << "%" << std::endl;
	}

	std::unique_ptr<CoordPlane> GCodeWriter::clone(
	    std::shared_ptr<CoordPlane> base) {
		std::unique_ptr<GCodeWriter> writer = std::make_unique<GCodeWriter>(
		    base->getPosition(), base->getSize(), this->trans, this->out);
		return writer;
	}

	bool GCodeWriter::jump(motor_point_t pnt, bool move) {
		coord_point_t point = this->trans->get(pnt);
		out << (move ? "G01" : "G00") << " X" << point.x << " Y" << point.y
		    << std::endl;
		return true;
	}
}  // namespace CalX
