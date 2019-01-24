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
	REQUIRE(std::get<OperatorType>(tokens[0].value) == OperatorType::OPENING_PARENTHESE);
	REQUIRE(tokens[1].type == TokenType::Integer);
	REQUIRE(std::get<int64_t>(tokens[1].value) == 2);
	REQUIRE(tokens[2].type == TokenType::Operator);
	REQUIRE(std::get<OperatorType>(tokens[2].value) == OperatorType::PLUS);
	REQUIRE(tokens[3].type == TokenType::Literal);
	REQUIRE(tokens[3].raw.compare("test") == 0);
	REQUIRE(tokens[4].type == TokenType::Operator);
	REQUIRE(std::get<OperatorType>(tokens[4].value) == OperatorType::SLASH);
	REQUIRE(tokens[5].type == TokenType::Integer);
	REQUIRE(std::get<int64_t>(tokens[5].value) == 500);
	REQUIRE(tokens[6].type == TokenType::Operator);
	REQUIRE(std::get<OperatorType>(tokens[6].value) == OperatorType::CLOSING_PARENTHESE);
	REQUIRE(tokens[7].type == TokenType::Operator);
	REQUIRE(std::get<OperatorType>(tokens[7].value) == OperatorType::POWER);
	REQUIRE(tokens[8].type == TokenType::Literal);
	REQUIRE(tokens[8].raw.compare("abcd") == 0);
	REQUIRE(tokens[9].type == TokenType::Operator);
	REQUIRE(std::get<OperatorType>(tokens[9].value) == OperatorType::MINUS);
	REQUIRE(tokens[10].type == TokenType::Real);
	REQUIRE(std::get<double>(tokens[10].value) == 3.14);
	REQUIRE(tokens[11].type == TokenType::Operator);
	REQUIRE(std::get<OperatorType>(tokens[11].value) == OperatorType::STAR);
	REQUIRE(tokens[12].type == TokenType::Operator);
	REQUIRE(std::get<OperatorType>(tokens[12].value) == OperatorType::COMMA);
}