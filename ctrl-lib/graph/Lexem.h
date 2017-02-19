#ifndef _8SMC1_CTRL_LIB_GRAPH_LEXEM_H_
#define _8SMC1_CTRL_LIB_GRAPH_LEXEM_H_

#include <string>
#include <cinttypes>

namespace _8SMC1 {

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