/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CALX_CTRL_LIB_GRAPH_AST_H_
#define CALX_CTRL_LIB_GRAPH_AST_H_

#include "calx/ctrl-lib/CtrlCore.h"
#include <cinttypes>
#include <string>
#include <memory>
#include <vector>
#include <iosfwd>

/* This file contains abstract syntax tree node definitions.
   Each node stores information about math expression and
   contains code to evaluate this expression using MathEngine and scope.*/

namespace CalX {

	enum class MathError {
		MNoError = 0,
		MNoVariable = 1,
		MNoFunction = 2,
		MWrongParameters = 3
	};

	struct engine_value_t {
		double value;
		MathError err;
	};
	class MathEngine;  // Forward referencing

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
		NodeType getType() const {
			return this->type;
		}
		virtual engine_value_t eval(MathEngine &) const = 0;

		friend std::ostream &operator<<(std::ostream &, const Node &);

	 protected:
		virtual void dump(std::ostream &) const = 0;

	 private:
		NodeType type;
	};

	class IntegerConstantNode : public Node {
	 public:
		IntegerConstantNode(int64_t v) : Node::Node(NodeType::IntegerConstant) {
			this->value = v;
		}
		int64_t getValue() const {
			return this->value;
		}
		engine_value_t eval(MathEngine &eng) const override {
			return { (double) this->value, MathError::MNoError };
		}

	 protected:
		void dump(std::ostream &) const override;

	 private:
		int64_t value;
	};

	class RealConstantNode : public Node {
	 public:
		RealConstantNode(double v) : Node::Node(NodeType::RealConstant) {
			this->value = v;
		}
		double getValue() const {
			return this->value;
		}
		engine_value_t eval(MathEngine &eng) const override {
			return { this->value, MathError::MNoError };
		}

	 protected:
		void dump(std::ostream &) const override;

	 private:
		double value;
	};

	class VariableNode : public Node {
	 public:
		VariableNode(const std::string &id)
		    : Node::Node(NodeType::Variable), id(id) {}
		const std::string &getId() const {
			return this->id;
		}
		engine_value_t eval(MathEngine &) const override;

	 protected:
		void dump(std::ostream &) const override;

	 private:
		std::string id;
	};

	class InvertNode : public Node {
	 public:
		InvertNode(std::unique_ptr<Node> n)
		    : Node::Node(NodeType::Invert), node(std::move(n)) {}
		engine_value_t eval(MathEngine &eng) const override {
			engine_value_t val = this->node->eval(eng);
			val.value *= -1;
			return val;
		}

	 protected:
		void dump(std::ostream &) const override;

	 private:
		std::unique_ptr<Node> node;
	};

	enum class BinaryOperation {
		Add = '+',
		Subtract = '-',
		Multiply = '*',
		Divide = '/',
		PowerOp = '^'
	};

	class BinaryNode : public Node {
	 public:
		BinaryNode(BinaryOperation, std::unique_ptr<Node>, std::unique_ptr<Node>);
		engine_value_t eval(MathEngine &eng) const override;

	 protected:
		void dump(std::ostream &) const override;

	 private:
		BinaryOperation oper;
		std::unique_ptr<Node> left;
		std::unique_ptr<Node> right;
	};

	class FunctionNode : public Node {
	 public:
		FunctionNode(const std::string &,
		             std::unique_ptr<std::vector<std::unique_ptr<Node>>>);
		engine_value_t eval(MathEngine &eng) const override;

	 protected:
		void dump(std::ostream &) const override;

	 private:
		std::string id;
		std::unique_ptr<std::vector<std::unique_ptr<Node>>> args;
	};
}  // namespace CalX

#endif
