#include <math.h>
#include "FunctionEngine.h"

namespace _8SMC1 {
	
	engine_value_t VariableNode::eval(FunctionEngine *eng) {
		return eng->getScope()->getVariable(id);
	}

	BinaryNode::BinaryNode(BinaryOperation op, Node *left, Node *right)
			: Node::Node(NodeType::Binary) {
		this->oper = op;
		this->left = left;
		this->right = right;
	}
		
	BinaryNode::~BinaryNode() {
		delete this->left;
		delete this->right;
	}
	
	engine_value_t BinaryNode::eval(FunctionEngine *eng) {
		engine_value_t res = {0, MathError::MNoError};
		engine_value_t leftv = eng->eval(this->left);
		if (leftv.err != MathError::MNoError) {
			res.err = leftv.err;
			return res;
		}
		engine_value_t rightv = eng->eval(this->right);
		if (rightv.err != MathError::MNoError) {
			res.err = rightv.err;
			return res;
		}
		long double left = leftv.value;
		long double right = rightv.value;
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
		
	FunctionNode::FunctionNode(std::string id, std::vector<Node*> *args)
			: Node::Node(NodeType::Function) {
		this->id = id;
		this->args = args;
	}
	
	FunctionNode::~FunctionNode() {
		for (size_t i = 0; i < this->args->size(); i++) {
			delete this->args->at(i);
		}
		delete this->args;
	}
		
	std::string FunctionNode::getId() {
		return this->id;
	}
		
	size_t FunctionNode::getArgumentCount() {
		return this->args->size();
	}
		
	Node *FunctionNode::getArgument(size_t i) {
		if (i >= this->args->size()) {
			return nullptr;
		}
		return this->args->at(i);
	}
	
	engine_value_t FunctionNode::eval(FunctionEngine *eng) {
		std::vector<long double> vec;
		for (size_t i = 0; i < this->args->size(); i++) {
			engine_value_t val = eng->eval(this->args->at(i));
			if (val.err != MathError::MNoError) {
				val.value = 0;
				return val;
			}
			vec.push_back(val.value);
		}
		return eng->getScope()->evalFunction(this->id, vec);
	}
}