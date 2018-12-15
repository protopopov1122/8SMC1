#include "catch.hpp"
#include "ctrl-lib/graph/FunctionParser.h"
#include "ctrl-lib/graph/MathEngine.h"
#include <sstream>

using namespace CalX;

TEST_CASE("Math expression lexer") {
	const std::string EXPRESSION =
	    "(2+ test\t/\n\n\n500)   ^\t\nabcd \t\n-3.14*,";
	std::stringstream input_stream(EXPRESSION);
	std::vector<Token> tokens;
	std::unique_ptr<Token> next_token;
	FunctionLexer lexer(input_stream);
	while ((next_token = lexer.lex()) != nullptr) {
		tokens.push_back(*next_token);
	}

	REQUIRE(tokens.size() == 13);
	REQUIRE(tokens[0].type == TokenType::Operator);
	REQUIRE(tokens[0].oper == OperatorType::OPENING_PARENTHESE);
	REQUIRE(tokens[1].type == TokenType::Integer);
	REQUIRE(tokens[1].integer == 2);
	REQUIRE(tokens[2].type == TokenType::Operator);
	REQUIRE(tokens[2].oper == OperatorType::PLUS);
	REQUIRE(tokens[3].type == TokenType::Literal);
	REQUIRE(tokens[3].literal.compare("test") == 0);
	REQUIRE(tokens[4].type == TokenType::Operator);
	REQUIRE(tokens[4].oper == OperatorType::SLASH);
	REQUIRE(tokens[5].type == TokenType::Integer);
	REQUIRE(tokens[5].integer == 500);
	REQUIRE(tokens[6].type == TokenType::Operator);
	REQUIRE(tokens[6].oper == OperatorType::CLOSING_PARENTHESE);
	REQUIRE(tokens[7].type == TokenType::Operator);
	REQUIRE(tokens[7].oper == OperatorType::POWER);
	REQUIRE(tokens[8].type == TokenType::Literal);
	REQUIRE(tokens[8].literal.compare("abcd") == 0);
	REQUIRE(tokens[9].type == TokenType::Operator);
	REQUIRE(tokens[9].oper == OperatorType::MINUS);
	REQUIRE(tokens[10].type == TokenType::Real);
	REQUIRE(tokens[10].real == 3.14);
	REQUIRE(tokens[11].type == TokenType::Operator);
	REQUIRE(tokens[11].oper == OperatorType::STAR);
	REQUIRE(tokens[12].type == TokenType::Operator);
	REQUIRE(tokens[12].oper == OperatorType::COMMA);
}