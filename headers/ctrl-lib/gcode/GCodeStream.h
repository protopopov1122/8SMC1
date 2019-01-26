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

#ifndef CALX_CTRL_LIB_GCODE_GCODE_LEXER_H_
#define CALX_CTRL_LIB_GCODE_GCODE_LEXER_H_

#include "ctrl-lib/CtrlCore.h"
#include <iosfwd>
#include <map>
#include <set>
#include <string>
#include <vector>

/* This is GCode parser that translates raw symbol stream into
   command sequence. Parser is quite liberal and doesn't strictly
   follow GCode standarts so it tries to execute even
   slightly incorrect GCode. */

namespace CalX {

	enum class GCodeParameterType { Integer, Real, Unknown };

	struct GCodeParameter {
		GCodeParameterType type;
		union {
			int64_t integer;
			double real;
		} value;
	};

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

	extern std::set<int16_t> GCODE_OPERATIONS;

	class GCodeCmd {
	 public:
		GCodeCmd(GCodeOperation);

		GCodeOperation getOperation() const;
		bool hasArgument(char) const;
		void putArgument(char, GCodeParameter);
		GCodeParameter getArgument(char);

	 private:
		GCodeOperation operation;
		std::map<char, GCodeParameter> args;
	};

	class GCodeToken {
	 public:
		GCodeToken(char, GCodeParameter);

		char getOperation() const;
		GCodeParameter getParameter() const;

	 private:
		char operation;
		GCodeParameter parameter;
	};

	class GCodeStream {
	 public:
		GCodeStream(std::istream &);

		void reset();
		bool hasNext();
		GCodeCmd next();

	 private:
		void find_lexems(std::istream &, std::vector<std::string> &);
		void find_tokens(std::vector<std::string> &, std::vector<GCodeToken> &);
		void find_commands(std::vector<GCodeToken> &, std::vector<GCodeCmd> &);
		bool parse_parameter(GCodeParameter &, const std::string &);
		std::vector<GCodeCmd> commands;
		std::size_t position;
	};
}  // namespace CalX

#endif