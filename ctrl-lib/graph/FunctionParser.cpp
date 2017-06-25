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


#include "ctrl-lib/graph/FunctionParser.h"
#include <iostream>

namespace CalX {

	FunctionParser::FunctionParser(FunctionLexer *lexer) {
		this->lexer = lexer;
		this->tokens[0] = this->lexer->lex();
		this->tokens[1] = this->lexer->lex();
		this->tokens[2] = this->lexer->lex();
		INIT_LOG("FunctionParser");
	}

	FunctionParser::~FunctionParser() {
		for (size_t i = 0; i < 3; i++) {
			if (this->tokens[i] != nullptr) {
				delete this->tokens[i];
			}
		}
		DESTROY_LOG("FunctionParser");
	}

	Node *FunctionParser::parse() {
		Node *node = nextAddsub();
		return node;
	}

	Node *FunctionParser::nextAddsub() {
		Node *node = nextMuldiv();
		if (node == nullptr) {
			return nullptr;
		}
		while (expectOperator(OperatorType::PLUS) ||
				expectOperator(OperatorType::MINUS)) {
			bool plus = this->tokens[0]->oper == OperatorType::PLUS;
			this->nextToken();
			Node *right = nextMuldiv();
			if (right == nullptr) {
				delete node;
				return nullptr;
			}
			node = new BinaryNode(plus ? BinaryOperation::Add : BinaryOperation::Subtract, node, right);
		}
		return node;
	}

	Node *FunctionParser::nextMuldiv() {
		Node *node = nextPower();
		if (node == nullptr) {
			return nullptr;
		}
		while (expectOperator(OperatorType::STAR) ||
				expectOperator(OperatorType::SLASH)) {
			bool mul = this->tokens[0]->oper == OperatorType::STAR;
			this->nextToken();
			Node *right = nextPower();
			if (right == nullptr) {
				delete node;
				return nullptr;
			}
			node = new BinaryNode(mul ? BinaryOperation::Multiply : BinaryOperation::Divide, node, right);
		}
		return node;
	}

	Node *FunctionParser::nextPower() {
		Node *node = nextFactor();
		if (node == nullptr) {
			return nullptr;
		}
		while (expectOperator(OperatorType::POWER)) {
			this->nextToken();
			Node *right = nextFactor();
			if (right == nullptr) {
				delete node;
				return nullptr;
			}
			node = new BinaryNode(BinaryOperation::PowerOp, node, right);
		}
		return node;
	}

	Node *FunctionParser::nextFactor() {
		if (this->tokens[0] != nullptr &&
			this->tokens[0]->type == TokenType::Integer) {
			IntegerConstantNode *node = new IntegerConstantNode(this->tokens[0]->integer);
			nextToken();
			return node;
		} else if (this->tokens[0] != nullptr &&
			this->tokens[0]->type == TokenType::Real) {
			RealConstantNode *node = new RealConstantNode(this->tokens[0]->real);
			nextToken();
			return node;
		} else if (this->tokens[0] != nullptr &&
			this->tokens[0]->type == TokenType::Literal) {
			std::string id = this->tokens[0]->literal;
			nextToken();
			if (expectOperator(OperatorType::OPENING_PARENTHESE)) {
				std::vector<Node*> *args = new std::vector<Node*>();
				nextToken();
				while (!expectOperator(OperatorType::CLOSING_PARENTHESE)) {
					if (this->tokens[0] == nullptr) {
						for (const auto& nd : *args) {
							delete nd;
						}
						return nullptr;
					}
					args->push_back(nextAddsub());
					if (expectOperator(OperatorType::COMMA)) {
						nextToken();
					} else if (!expectOperator(OperatorType::CLOSING_PARENTHESE)) {
						for (const auto& nd : *args) {
							delete nd;
						}
						delete args;
						return nullptr;
					}
				}
				nextToken();
				FunctionNode *node = new FunctionNode(id, args);
				return node;
			}
			VariableNode *node = new VariableNode(id);
			return node;
		} else if (expectOperator(OperatorType::PLUS)) {
			nextToken();
			return nextAddsub();
		} else if (expectOperator(OperatorType::MINUS)) {
			nextToken();
			Node *node = nextAddsub();
			return node != nullptr ? new InvertNode(node) : nullptr;
		} else if (expectOperator(OperatorType::OPENING_PARENTHESE)) {
			nextToken();
			Node *node = nextAddsub();
			if (!expectOperator(OperatorType::CLOSING_PARENTHESE)) {
				delete node;
				return nullptr;
			}
			nextToken();
			return node;
		}
		return nullptr;
	}

	Token *FunctionParser::nextToken() {
		if (this->tokens[0] != nullptr) {
			delete this->tokens[0];
		}
		this->tokens[0] = this->tokens[1];
		this->tokens[1] = this->tokens[2];
		this->tokens[2] = this->lexer->lex();
		return this->tokens[0];
	}

	bool FunctionParser::expectOperator(Token *tok, OperatorType op) {
		return tok != nullptr &&
				tok->type == TokenType::Operator &&
				tok->oper == op;
	}

	bool FunctionParser::expectOperator(OperatorType op) {
		return expectOperator(this->tokens[0], op);
	}
}
