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


#ifndef CALX_CTRL_LIB_GRAPH_AST_H_
#define CALX_CTRL_LIB_GRAPH_AST_H_

#include <vector>
#include <string>
#include <cinttypes>

/* This file contains abstract syntax tree node definitions.
   Each node stores information about math expression and
   contains code to evaluate this expression using FunctionEngine and scope.*/

namespace CalX {
	
	enum MathError {
		MNoError, MNoVariable, MNoFunction,
		MWrongParameters
	};
	
	struct engine_value_t {
		long double value;
		MathError err;
	};
	class FunctionEngine; // Forward referencing
	
	enum NodeType {
		IntegerConstant, RealConstant,
		Variable, Function,
		Binary
	};
	
	class Node {
		public:
			Node(NodeType tp) {this->type = tp;}
			virtual ~Node() {}
			NodeType getType() {return this->type;}
			virtual engine_value_t eval(FunctionEngine*) = 0;
		private:
			NodeType type;
	};
	
	class IntegerConstantNode : public Node {
		public:
			IntegerConstantNode(int64_t v) : Node::Node(NodeType::IntegerConstant) {this->value = v;}
			virtual ~IntegerConstantNode() {}
			int64_t getValue() {return this->value;}
			virtual engine_value_t eval(FunctionEngine *eng) {return {(long double) this->value, MathError::MNoError};}
		private:
			int64_t value;
	};
	
	class RealConstantNode : public Node {
		public:
			RealConstantNode(long double v) : Node::Node(NodeType::RealConstant) {this->value = v;}
			virtual ~RealConstantNode() {}
			long double getValue() {return this->value;}
			virtual engine_value_t eval(FunctionEngine *eng) {return {this->value, MathError::MNoError};}
		private:
			long double value;
	};
	
	class VariableNode : public Node {
		public:
			VariableNode(std::string i) : Node::Node(NodeType::Variable) {this->id = i;};
			virtual ~VariableNode() {}
			std::string getId() {return this->id;}
			virtual engine_value_t eval(FunctionEngine*);
		private:
			std::string id;
	};
	
	enum BinaryOperation {
		Add, Subtract, Multiply, Divide, PowerOp
	};
	
	class BinaryNode : public Node {
		public:
			BinaryNode(BinaryOperation, Node*, Node*);
			virtual ~BinaryNode();
			BinaryOperation getOperation() {return this->oper;}
			Node *getLeft() {return this->left;}
			Node *getRight() {return this->right;}
			virtual engine_value_t eval(FunctionEngine *eng);
		private:
			BinaryOperation oper;
			Node *left;
			Node *right;
	};
	
	class FunctionNode : public Node {
		public:
			FunctionNode(std::string, std::vector<Node*>*);
			virtual ~FunctionNode();
			std::string getId();
			size_t getArgumentCount();
			Node *getArgument(size_t);
			virtual engine_value_t eval(FunctionEngine *eng);
		private:
			std::string id;
			std::vector<Node*> *args;
	};
}

#endif