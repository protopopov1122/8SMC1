/*
        Copyright (c) 2017 Jevgenijs Protopopovs

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

#include "ctrl-lib/graph/FunctionParser.h"
#include <iostream>

namespace CalX {

	FunctionParser::FunctionParser(std::unique_ptr<FunctionLexer> lexer) {
		this->lexer = std::move(lexer);
		this->tokens[0] = this->lexer->lex();
		this->tokens[1] = this->lexer->lex();
		this->tokens[2] = this->lexer->lex();
		INIT_LOG("FunctionParser");
	}

	FunctionParser::~FunctionParser() {
		DESTROY_LOG("FunctionParser");
	}

	std::unique_ptr<Node> FunctionParser::parse() {
		return nextAddsub();
	}

	std::unique_ptr<Node> FunctionParser::nextAddsub() {
		std::unique_ptr<Node> node = nextMuldiv();
		if (node == nullptr) {
			return nullptr;
		}
		while (expectOperator(OperatorType::PLUS) ||
		       expectOperator(OperatorType::MINUS)) {
			bool plus = this->tokens[0]->oper == OperatorType::PLUS;
			this->nextToken();
			std::unique_ptr<Node> right = nextMuldiv();
			if (right == nullptr) {
				return nullptr;
			}
			std::unique_ptr<Node> res = std::make_unique<BinaryNode>(
			    plus ? BinaryOperation::Add : BinaryOperation::Subtract,
			    std::move(node), std::move(right));
			node = std::move(res);
		}
		return node;
	}

	std::unique_ptr<Node> FunctionParser::nextMuldiv() {
		std::unique_ptr<Node> node = nextPower();
		if (node == nullptr) {
			return nullptr;
		}
		while (expectOperator(OperatorType::STAR) ||
		       expectOperator(OperatorType::SLASH)) {
			bool mul = this->tokens[0]->oper == OperatorType::STAR;
			this->nextToken();
			std::unique_ptr<Node> right = nextPower();
			if (right == nullptr) {
				return nullptr;
			}
			std::unique_ptr<Node> res = std::make_unique<BinaryNode>(
			    mul ? BinaryOperation::Multiply : BinaryOperation::Divide,
			    std::move(node), std::move(right));
			node = std::move(res);
		}
		return node;
	}

	std::unique_ptr<Node> FunctionParser::nextPower() {
		std::unique_ptr<Node> node = nextFactor();
		if (node == nullptr) {
			return nullptr;
		}
		while (expectOperator(OperatorType::POWER)) {
			this->nextToken();
			std::unique_ptr<Node> right = nextFactor();
			if (right == nullptr) {
				return nullptr;
			}
			std::unique_ptr<Node> res = std::make_unique<BinaryNode>(
			    BinaryOperation::PowerOp, std::move(node), std::move(right));
			node = std::move(res);
		}
		return node;
	}

	std::unique_ptr<Node> FunctionParser::nextFactor() {
		if (this->tokens[0] != nullptr &&
		    this->tokens[0]->type == TokenType::Integer) {
			std::unique_ptr<IntegerConstantNode> node =
			    std::make_unique<IntegerConstantNode>(this->tokens[0]->integer);
			nextToken();
			return node;
		} else if (this->tokens[0] != nullptr &&
		           this->tokens[0]->type == TokenType::Real) {
			std::unique_ptr<RealConstantNode> node =
			    std::make_unique<RealConstantNode>(this->tokens[0]->real);
			nextToken();
			return node;
		} else if (this->tokens[0] != nullptr &&
		           this->tokens[0]->type == TokenType::Literal) {
			std::string id = this->tokens[0]->literal;
			nextToken();
			if (expectOperator(OperatorType::OPENING_PARENTHESE)) {
				std::unique_ptr<std::vector<std::unique_ptr<Node>>> args =
				    std::make_unique<std::vector<std::unique_ptr<Node>>>();
				nextToken();
				while (!expectOperator(OperatorType::CLOSING_PARENTHESE)) {
					if (this->tokens[0] == nullptr) {
						return nullptr;
					}
					args->push_back(nextAddsub());
					if (expectOperator(OperatorType::COMMA)) {
						nextToken();
					} else if (!expectOperator(OperatorType::CLOSING_PARENTHESE)) {
						return nullptr;
					}
				}
				nextToken();
				std::unique_ptr<FunctionNode> node =
				    std::make_unique<FunctionNode>(id, std::move(args));
				return node;
			}
			std::unique_ptr<VariableNode> node = std::make_unique<VariableNode>(id);
			return node;
		} else if (expectOperator(OperatorType::PLUS)) {
			nextToken();
			return nextAddsub();
		} else if (expectOperator(OperatorType::MINUS)) {
			nextToken();
			std::unique_ptr<Node> node = nextAddsub();
			return node != nullptr ? std::make_unique<InvertNode>(std::move(node))
			                       : nullptr;
		} else if (expectOperator(OperatorType::OPENING_PARENTHESE)) {
			nextToken();
			std::unique_ptr<Node> node = nextAddsub();
			if (!expectOperator(OperatorType::CLOSING_PARENTHESE)) {
				return nullptr;
			}
			nextToken();
			return node;
		}
		return nullptr;
	}

	void FunctionParser::nextToken() {
		this->tokens[0] = std::move(this->tokens[1]);
		this->tokens[1] = std::move(this->tokens[2]);
		this->tokens[2] = this->lexer->lex();
	}

	bool FunctionParser::expectOperator(Token *tok, OperatorType op) {
		return tok != nullptr && tok->type == TokenType::Operator &&
		       tok->oper == op;
	}

	bool FunctionParser::expectOperator(OperatorType op) {
		return expectOperator(this->tokens[0].get(), op);
	}
}
