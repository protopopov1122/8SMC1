#include "FunctionEngine.h"

namespace _8SMC1 {
	
	EngineScope::EngineScope() {
		
	}
	
	EngineScope::~EngineScope() {
		for (const auto& kv : this->func) {
			delete kv.second;
		}
	}
	
	engine_value_t EngineScope::getVariable(std::string id) {
		if (this->vars.count(id) == 0) {
			return 0;
		}
		return this->vars[id];
	}
	
	bool EngineScope::hasVariable(std::string id) {
		return this->vars.count(id) != 0;
	}
	
	void EngineScope::putVariable(std::string id, engine_value_t val) {
		this->vars[id] = val;
	}
	
	bool EngineScope::hasFunction(std::string id) {
		return this->func.count(id) != 0;
	}
	
	bool EngineScope::addFunction(std::string id, EngineFunction *func) {
		if (this->func.count(id) != 0) {
			return false;
		}
		this->func[id] = func;
		return true;
	}
	
	engine_value_t EngineScope::evalFunction(std::string id, std::vector<engine_value_t> &args) {
		if (this->func.count(id) == 0) {
			return 0;
		}
		EngineFunction *fun = this->func[id];
		return fun->eval(args);
	}
	
	FunctionEngine::FunctionEngine() {
		
	}
	
	FunctionEngine::~FunctionEngine() {
		
	}
	
	EngineScope *FunctionEngine::getScope() {
		return &this->scope;
	}
	
	engine_value_t FunctionEngine::eval(Node *node) {
		return node->eval(this);
	}
}