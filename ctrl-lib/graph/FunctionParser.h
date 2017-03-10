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