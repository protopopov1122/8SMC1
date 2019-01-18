/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

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

#ifndef CALX_CTRL_LIB_GCODE_GCODE_INTERPRETER_H_
#define CALX_CTRL_LIB_GCODE_GCODE_INTERPRETER_H_

#include "ctrl-lib/CtrlCore.h"
#include "ctrl-lib/conf/Dictionary.h"
#include "ctrl-lib/gcode/GCodeStream.h"
#include "ctrl-lib/plane/CoordPlane.h"
#include "ctrl-lib/translator/CoordTranslator.h"

/* GCode interpreter executes parser produced commands
   on given coordinate plane with given speeed. */

namespace CalX {
	class GCodeInterpreter {
	 public:
		static ErrorCode execute(GCodeStream &, CoordPlane &,
		                         std::shared_ptr<CoordTranslator>,
		                         ConfigurationCatalogue &, float, TaskState &);
	};
}  // namespace CalX

#endif
