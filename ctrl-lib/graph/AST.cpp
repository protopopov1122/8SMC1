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
		engine_value_t left = eng->eval(this->left);
		engine_value_t right = eng->eval(this->right);
		engine_value_t res = 0;
		switch (this->oper) {
			case BinaryOperation::Add:
				res = left + right;
			break;
			case BinaryOperation::Subtract:
				res = left - right;
			break;
			case BinaryOperation::Multiply:
				res = left * right;
			break;
			case BinaryOperation::Divide:
				res = left / right;
			break;
			case BinaryOperation::PowerOp:
				res = pow(left, right);
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
		std::vector<engine_value_t> vec;
		for (size_t i = 0; i < this->args->size(); i++) {
			vec.push_back(eng->eval(this->args->at(i)));
		}
		return eng->getScope()->evalFunction(this->id, vec);
	}
}