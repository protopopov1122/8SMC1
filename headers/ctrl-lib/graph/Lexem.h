/*
	Copyright (c) 2017 Jevgenijs Protopopovs

	This file is part of CalX project.

	CalX is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
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

#include <string>
#include <cinttypes>

/* This file contains definitions used by lexer. */

namespace CalX {

  enum TokenType { Operator, Literal, Integer, Real };

  enum OperatorType {
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
	OperatorType oper;
	int64_t integer;
	double real;
	std::string literal;
  };
}

#endif