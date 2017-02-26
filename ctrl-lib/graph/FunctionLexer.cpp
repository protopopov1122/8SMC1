#include "FunctionParser.h"
#include <string.h>
#include <cinttypes>
#include <stdio.h>

namespace _8SMC1 {
	
	FunctionLexer::FunctionLexer(std::istream &is) {
		this->in = &is;
	}
	
	FunctionLexer::~FunctionLexer() {
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