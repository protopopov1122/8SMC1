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

#include "calx/ctrl-lib/CtrlCore.h"
#include "calx/ctrl-lib/conf/Dictionary.h"
#include "calx/ctrl-lib/plane/CoordPlane.h"
#include "calx/ctrl-lib/translator/CoordTranslator.h"
#include "gcodelib/runtime/Interpreter.h"
#include <set>

/* GCode interpreter executes parser produced commands
   on given coordinate plane with given speeed. */

namespace gclr = GCodeLib::Runtime;

namespace CalX {

	enum class GCodeOperation {
		None = -1,
		RapidMove = 0,
		LinearMove = 1,
		ClockwiseArc = 2,
		CounterClockwiseArc = 3,
		SwitchInches = 20,
		SwitchMillimeters = 21,
		Home = 28,
		AbsolutePositioning = 90,
		RelativePositioning = 91,
		SetPosition = 92
	};

	class GCodeInterpreter : public gclr::GCodeInterpreter {
	 public:
                GCodeInterpreter(gclr::GCodeIRModule &, CoordPlane &, std::shared_ptr<CoordTranslator>, ConfigurationCatalogue &, float, TaskState &);
                ErrorCode run();
         protected:
              void syscall(gclr::GCodeSyscallType, const gclr::GCodeRuntimeValue &, const gclr::GCodeScopedDictionary<unsigned char> &) override;
         private:
                CoordPlane &plane;
                std::shared_ptr<CoordTranslator> trans;
                ConfigurationCatalogue &config;
                float speed;
                TaskState &state;

                std::shared_ptr<LinearCoordTranslator> unit_trans;
                std::shared_ptr<LinearCoordTranslator> translator;
                motor_point_t offset;
                int invert;
                bool relative_pos;
	};
}  // namespace CalX

#endif
