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
#include "ctrl-lib/CtrlCore.h"

/* This file contains abstract syntax tree node definitions.
   Each node stores information about math expression and
   contains code to evaluate this expression using FunctionEngine and scope.*/

namespace CalX {

  enum MathError {
	MNoError = 0,
	MNoVariable = 1,
	MNoFunction = 2,
	MWrongParameters = 3
  };

  struct engine_value_t {
	double value;
	MathError err;
  };
  class FunctionEngine;  // Forward referencing

  enum class NodeType {
	IntegerConstant,
	RealConstant,
	Variable,
	Function,
	Binary,
	Invert
  };

  class Node {
   public:
	Node(NodeType tp) {
	  this->type = tp;
	  INIT_LOG("Node" + std::to_string(static_cast<int>(tp)));
	}
	virtual ~Node() {
	  DESTROY_LOG("Node" + std::to_string(static_cast<int>(type)));
	}
	NodeType getType() {
	  return this->type;
	}
	virtual engine_value_t eval(FunctionEngine *) = 0;

   private:
	NodeType type;
  };

  class IntegerConstantNode : public Node {
   public:
	IntegerConstantNode(int64_t v) : Node::Node(NodeType::IntegerConstant) {
	  this->value = v;
	}
	virtual ~IntegerConstantNode() {}
	int64_t getValue() {
	  return this->value;
	}
	virtual engine_value_t eval(FunctionEngine *eng) {
	  return { (double) this->value, MathError::MNoError };
	}

   private:
	int64_t value;
  };

  class RealConstantNode : public Node {
   public:
	RealConstantNode(double v) : Node::Node(NodeType::RealConstant) {
	  this->value = v;
	}
	virtual ~RealConstantNode() {}
	double getValue() {
	  return this->value;
	}
	virtual engine_value_t eval(FunctionEngine *eng) {
	  return { this->value, MathError::MNoError };
	}

   private:
	double value;
  };

  class VariableNode : public Node {
   public:
	VariableNode(std::string i) : Node::Node(NodeType::Variable) {
	  this->id = i;
	}
	virtual ~VariableNode() {}
	std::string getId() {
	  return this->id;
	}
	virtual engine_value_t eval(FunctionEngine *);

   private:
	std::string id;
  };

  class InvertNode : public Node {
   public:
	InvertNode(Node *n) : Node::Node(NodeType::Invert) {
	  this->node = n;
	}
	virtual ~InvertNode() {
	  delete this->node;
	}
	Node *getExpression() {
	  return this->node;
	}
	virtual engine_value_t eval(FunctionEngine *eng) {
	  engine_value_t val = this->node->eval(eng);
	  val.value *= -1;
	  return val;
	}

   private:
	Node *node;
  };

  enum BinaryOperation { Add, Subtract, Multiply, Divide, PowerOp };

  class BinaryNode : public Node {
   public:
	BinaryNode(BinaryOperation, Node *, Node *);
	virtual ~BinaryNode();
	BinaryOperation getOperation() {
	  return this->oper;
	}
	Node *getLeft() {
	  return this->left;
	}
	Node *getRight() {
	  return this->right;
	}
	virtual engine_value_t eval(FunctionEngine *eng);

   private:
	BinaryOperation oper;
	Node *left;
	Node *right;
  };

  class FunctionNode : public Node {
   public:
	FunctionNode(std::string, std::vector<Node *> *);
	virtual ~FunctionNode();
	std::string getId();
	size_t getArgumentCount();
	Node *getArgument(size_t);
	virtual engine_value_t eval(FunctionEngine *eng);

   private:
	std::string id;
	std::vector<Node *> *args;
  };
}

#endif
