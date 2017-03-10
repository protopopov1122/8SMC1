#ifndef CALX_CTRL_LIB_GRAPH_LEXEM_H_
#define CALX_CTRL_LIB_GRAPH_LEXEM_H_

#include <string>
#include <cinttypes>

/* This file contains definitions used by lexer. */

namespace CalX {

	enum TokenType {
		Operator, Literal, Integer, Real
	};
	
	enum OperatorType {
		OPENING_PARENTHESE, CLOSING_PARENTHESE,
		PLUS, MINUS, STAR, SLASH, POWER, COMMA
	};

	struct Token {
		TokenType type;
		OperatorType oper;
		int64_t integer;
		long double real;
		std::string literal;
	};
}

#endif