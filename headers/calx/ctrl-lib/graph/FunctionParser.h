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

#ifndef CALX_CTRL_LIB_GRAPH_FUNCTION_PARSER_H_
#define CALX_CTRL_LIB_GRAPH_FUNCTION_PARSER_H_

#include "calx/ctrl-lib/graph/AST.h"
#include "calx/ctrl-lib/graph/Lexem.h"
#include <cinttypes>
#include <memory>
#include <iosfwd>
#include <map>
#include <string>

namespace CalX {

	class FunctionLexer {
	 public:
		FunctionLexer(std::istream &);
		std::unique_ptr<Token> lex() const;

	 private:
		std::istream &in;
	};

	class FunctionParser {
	 public:
		FunctionParser(std::unique_ptr<FunctionLexer>);

		std::unique_ptr<Node> parse();

	 private:
		void nextToken();
		std::unique_ptr<Node> nextAddsub();
		std::unique_ptr<Node> nextMuldiv();
		std::unique_ptr<Node> nextPower();
		std::unique_ptr<Node> nextFactor();

		bool expectOperator(Token *, OperatorType);
		bool expectOperator(OperatorType);

		std::unique_ptr<FunctionLexer> lexer;
		std::map<uint32_t, std::string> symbols;
		std::unique_ptr<Token> tokens[3];
	};
}  // namespace CalX

#endif