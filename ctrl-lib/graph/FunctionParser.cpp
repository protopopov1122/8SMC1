#include "FunctionParser.h"
#include <iostream>

namespace _8SMC1 {
	
	FunctionParser::FunctionParser(FunctionLexer *lexer) {
		this->lexer = lexer;
		this->tokens[0] = this->lexer->lex();
		this->tokens[1] = this->lexer->lex();
		this->tokens[2] = this->lexer->lex();
	}
	
	FunctionParser::~FunctionParser() {
		for (size_t i = 0; i < 3; i++) {
			if (this->tokens[i] != nullptr) {
				delete this->tokens[i];
			}
		}
	}
	
	Node *FunctionParser::parse() {
		return nextAddsub();
	}
	
	Node *FunctionParser::nextAddsub() {
		Node *node = nextMuldiv();
		while (expectOperator(OperatorType::PLUS) ||
				expectOperator(OperatorType::MINUS)) {
			bool plus = this->tokens[0]->oper == OperatorType::PLUS;
			this->nextToken();
			Node *right = nextMuldiv();
			node = new BinaryNode(plus ? BinaryOperation::Add : BinaryOperation::Subtract, node, right);
		}
		return node;
	}
	
	Node *FunctionParser::nextMuldiv() {
		Node *node = nextPower();
		while (expectOperator(OperatorType::STAR) ||
				expectOperator(OperatorType::SLASH)) {
			bool mul = this->tokens[0]->oper == OperatorType::STAR;
			this->nextToken();
			Node *right = nextPower();
			node = new BinaryNode(mul ? BinaryOperation::Multiply : BinaryOperation::Divide, node, right);
		}
		return node;
	}
	
	Node *FunctionParser::nextPower() {
		Node *node = nextFactor();
		while (expectOperator(OperatorType::POWER)) {
			this->nextToken();
			Node *right = nextFactor();
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
				while (this->tokens[0] != nullptr &&
					!expectOperator(OperatorType::CLOSING_PARENTHESE)) {
					args->push_back(nextAddsub());
					if (expectOperator(OperatorType::COMMA)) {
						nextToken();	
					}
				}
				nextToken();
				FunctionNode *node = new FunctionNode(id, args);
				return node;
			}
			VariableNode *node = new VariableNode(id);
			return node;
		} else if (expectOperator(OperatorType::OPENING_PARENTHESE)) {
			nextToken();
			Node *node = nextAddsub();
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