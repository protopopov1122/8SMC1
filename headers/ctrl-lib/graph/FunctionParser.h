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


#ifndef CALX_CTRL_LIB_GRAPH_FUNCTION_PARSER_H_
#define CALX_CTRL_LIB_GRAPH_FUNCTION_PARSER_H_

#include <iostream>
#include <map>
#include <string>
#include <cinttypes>
#include "Lexem.h"
#include "AST.h"

namespace CalX {
	
	class FunctionLexer {
		public:
			FunctionLexer(std::istream&);
			virtual ~FunctionLexer();
			Token *lex();
		private:
			std::istream *in;
	};
	
	class FunctionParser {
		public:
			FunctionParser(FunctionLexer*);
			virtual ~FunctionParser();
			
			Node *parse();
		private:
			Token *nextToken();
			Node *nextAddsub();
			Node *nextMuldiv();
			Node *nextPower();
			Node *nextFactor();
			
			bool expectOperator(Token*, OperatorType);
			bool expectOperator(OperatorType);
		
			FunctionLexer *lexer;
			std::map<uint32_t, std::string> symbols;
			Token *tokens[3];
	};
}

#endif