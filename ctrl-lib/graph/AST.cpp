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

#include "ctrl-lib/graph/FunctionEngine.h"
#include <math.h>

namespace CalX {

	engine_value_t VariableNode::eval(FunctionEngine *eng) {
		return eng->getScope()->getVariable(id);
	}

	BinaryNode::BinaryNode(BinaryOperation op, std::unique_ptr<Node> left,
	                       std::unique_ptr<Node> right)
	    : Node::Node(NodeType::Binary),
	      oper(op),
	      left(std::move(left)),
	      right(std::move(right)) {}

	engine_value_t BinaryNode::eval(FunctionEngine *eng) {
		engine_value_t res = { 0, MathError::MNoError };
		engine_value_t leftv = eng->eval(this->left.get());
		if (leftv.err != MathError::MNoError) {
			res.err = leftv.err;
			return res;
		}
		engine_value_t rightv = eng->eval(this->right.get());
		if (rightv.err != MathError::MNoError) {
			res.err = rightv.err;
			return res;
		}
		double left = leftv.value;
		double right = rightv.value;
		switch (this->oper) {
			case BinaryOperation::Add:
				res.value = left + right;
				break;
			case BinaryOperation::Subtract:
				res.value = left - right;
				break;
			case BinaryOperation::Multiply:
				res.value = left * right;
				break;
			case BinaryOperation::Divide:
				res.value = left / right;
				break;
			case BinaryOperation::PowerOp:
				res.value = pow(left, right);
				break;
		}
		return res;
	}

	FunctionNode::FunctionNode(
	    std::string id, std::unique_ptr<std::vector<std::unique_ptr<Node>>> args)
	    : Node::Node(NodeType::Function), id(id), args(std::move(args)) {}

	engine_value_t FunctionNode::eval(FunctionEngine *eng) {
		std::vector<double> vec;
		for (size_t i = 0; i < this->args->size(); i++) {
			engine_value_t val = eng->eval(this->args->at(i).get());
			if (val.err != MathError::MNoError) {
				val.value = 0;
				return val;
			}
			vec.push_back(val.value);
		}
		return eng->getScope()->evalFunction(this->id, vec);
	}
}
