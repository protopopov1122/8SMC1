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

#include "calx/ctrl-lib/graph/FunctionParser.h"
#include <cinttypes>
#include <cstring>

namespace CalX {

	FunctionLexer::FunctionLexer(std::istream &is) : in(is) {}

	std::unique_ptr<Token> FunctionLexer::lex() const {
		int chr = in.get();
		while (isspace(chr)) {
			if (chr == EOF) {
				return nullptr;
			}
			chr = in.get();
		}
		if (chr == EOF) {
			return nullptr;
		}
		switch (chr) {
#define OPER(ch, tp)                                                           \
	case ch: {                                                                   \
		char buf[] = { (char) chr, '\0' };                                         \
		std::string raw(buf);                                                      \
		std::unique_ptr<Token> lexem = std::make_unique<Token>();                  \
		lexem->raw = raw;                                                          \
		lexem->value.emplace<OperatorType>(tp);                                    \
		lexem->type = TokenType::Operator;                                         \
		return lexem;                                                              \
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
		const std::size_t BUF_SZ = 256;
		std::size_t offset = 0;
		char buf[BUF_SZ];
		if (isdigit(chr)) {
			bool real = false;
			for (; (isdigit(chr) || chr == '.') && offset + 1 < BUF_SZ;
			     chr = in.get()) {
				if (chr == '.') {
					real = true;
				}
				buf[offset++] = (char) chr;
			}
			in.unget();
			buf[offset] = '\0';
			std::string raw(buf);
			std::unique_ptr<Token> lexem = std::make_unique<Token>();
			lexem->raw = raw;
			if (real) {
				lexem->type = TokenType::Real;
				lexem->value.emplace<double>(std::stod(raw));
			} else {
				lexem->type = TokenType::Integer;
				lexem->value.emplace<int64_t>(std::stoi(raw));
			}
			return lexem;
		}
		while (offset + 1 < BUF_SZ) {
			buf[offset++] = (char) chr;

			chr = in.get();
			if (chr == EOF) {
				break;
			}
			if (isspace(chr) || isdigit(chr) || chr == '+' || chr == '-' ||
			    chr == '*' || chr == '/' || chr == '(' || chr == ')' || chr == '^' ||
			    chr == ',') {
				in.unget();
				break;
			}
		}
		buf[offset] = '\0';
		std::string raw(buf);
		std::unique_ptr<Token> lexem = std::make_unique<Token>();
		lexem->type = TokenType::Literal;
		lexem->raw = raw;
		lexem->value.emplace<std::reference_wrapper<std::string>>(
		    std::ref(lexem->raw));
		return lexem;
	}
}  // namespace CalX
