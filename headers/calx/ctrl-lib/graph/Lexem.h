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

#ifndef CALX_CTRL_LIB_GRAPH_LEXEM_H_
#define CALX_CTRL_LIB_GRAPH_LEXEM_H_

#include <cinttypes>
#include <string>
#include <variant>
#include <functional>

/* This file contains definitions used by lexer. */

namespace CalX {

	enum class TokenType { Operator = 0, Literal = 1, Integer = 2, Real = 3 };

	enum class OperatorType {
		OPENING_PARENTHESE,
		CLOSING_PARENTHESE,
		PLUS,
		MINUS,
		STAR,
		SLASH,
		POWER,
		COMMA
	};

	struct Token {
		TokenType type;
		std::variant<OperatorType, int64_t, double,
		             std::reference_wrapper<std::string>>
		    value;
		std::string raw;
	};
}  // namespace CalX

#endif