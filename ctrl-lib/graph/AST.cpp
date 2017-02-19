#include "AST.h"

namespace _8SMC1 {

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
		
		FunctionNode::FunctionNode(uint32_t id, std::vector<Node*> *args)
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
		
		uint32_t FunctionNode::getId() {
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
}