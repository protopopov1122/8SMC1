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


#include "FunctionParser.h"
#include <string.h>
#include <cinttypes>
#include <stdio.h>

namespace CalX {
	
	FunctionLexer::FunctionLexer(std::istream &is) {
		this->in = &is;
		INIT_LOG("FunctionLexer");
	}
	
	FunctionLexer::~FunctionLexer() {
		DESTROY_LOG("FunctionLexer");
	}
	
	Token *FunctionLexer::lex() {
		int chr = in->get();
		while (isspace(chr)) {
			if (chr == EOF) {
				return nullptr;
			}
			chr = in->get();
		}
		if (chr == EOF) {
			return nullptr;
		}
		switch (chr) {
			#define OPER(ch, tp) case ch: {\
			                        char buf[] = {(char) chr, '\0'};\
									std::string raw(buf);\
									Token *lexem = new Token();\
									lexem->literal = raw;\
									lexem->oper = tp;\
									lexem->type = TokenType::Operator;\
									return lexem;\
			                     } break;
								 
			OPER('+', OperatorType::PLUS)
			OPER('-', OperatorType::MINUS)
			OPER('*', OperatorType::STAR)
			OPER('/', OperatorType::SLASH)
			OPER('(', OperatorType::OPENING_PARENTHESE)
			OPER(')', OperatorType::CLOSING_PARENTHESE)
			OPER('^', OperatorType::POWER)
			OPER(',', OperatorType::COMMA)
			#undef OPER
		}
		const size_t BUF_SZ = 256;
		size_t offset = 0;
		char buf[BUF_SZ];
		if (isdigit(chr)) {
			bool real = false;
			for (; (isdigit(chr) || chr == '.') && offset + 1 < BUF_SZ;
				chr = in->get()) {
				if (chr == '.') {
					real = true;
				}
				buf[offset++] = chr;
			}
			in->unget();
			buf[offset] = '\0';
			std::string raw(buf);
			Token *lexem = new Token();
			lexem->literal = raw;
			if (real) {
				lexem->type = TokenType::Real;
				lexem->real = std::stold(raw);
			} else {
				lexem->type = TokenType::Integer;
				lexem->integer = std::stoi(raw);
			}
			return lexem;
		}
		while (offset + 1 < BUF_SZ) {
			buf[offset++] = chr;
			
			chr = in->get();
			if (chr == EOF) {
				break;
			}
			if (isspace(chr) ||
				isdigit(chr) ||
				chr == '+' ||
				chr == '-' ||
				chr == '*' ||
				chr == '/' ||
				chr == '(' ||
				chr == ')' ||
				chr == '^' ||
				chr == ',') {
				in->unget();
				break;
			}
		}
		buf[offset] = '\0';
		std::string raw(buf);
		Token *lexem = new Token();
		lexem->type = TokenType::Literal;
		lexem->literal = raw;
		return lexem;
	}
}