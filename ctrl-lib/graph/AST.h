#ifndef _8SMC1_CTRL_LIB_GRAPH_AST_H_
#define _8SMC1_CTRL_LIB_GRAPH_AST_H_

#include <vector>
#include <cinttypes>

namespace _8SMC1 {
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
		private:
			NodeType type;
	};
	
	class IntegerConstantNode : public Node {
		public:
			IntegerConstantNode(int64_t v) : Node::Node(NodeType::IntegerConstant) {this->value = v;}
			virtual ~IntegerConstantNode() {}
			int64_t getValue() {return this->value;}
		private:
			int64_t value;
	};
	
	class RealConstantNode : public Node {
		public:
			RealConstantNode(long double v) : Node::Node(NodeType::RealConstant) {this->value = v;}
			virtual ~RealConstantNode() {}
			long double getValue() {return this->value;}
		private:
			long double value;
	};
	
	class VariableNode : public Node {
		public:
			VariableNode(uint32_t i) : Node::Node(NodeType::Variable) {this->id = i;};
			virtual ~VariableNode() {}
			uint32_t getId() {return this->id;}
		private:
			uint32_t id;
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
		private:
			BinaryOperation oper;
			Node *left;
			Node *right;
	};
	
	class FunctionNode : public Node {
		public:
			FunctionNode(uint32_t, std::vector<Node*>*);
			virtual ~FunctionNode();
			uint32_t getId();
			size_t getArgumentCount();
			Node *getArgument(size_t);
		private:
			uint32_t id;
			std::vector<Node*> *args;
	};
}

#endif