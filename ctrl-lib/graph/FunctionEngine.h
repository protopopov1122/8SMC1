#ifndef _8SMC1_CTRL_LIB_GRAPH_FUNCTION_ENGINE_H_
#define _8SMC1_CTRL_LIB_GRAPH_FUNCTION_ENGINE_H_

#include <map>
#include <vector>
#include <string>
#include <cinttypes>
#include "AST.h"
#include "ctrl-lib/CtrlCore.h"

namespace _8SMC1 {
		
	class EngineFunction {
		public:
			virtual ~EngineFunction() {}
			virtual engine_value_t eval(std::vector<long double>&) = 0;
	};
	
	class EngineScope {
		public:
			EngineScope();
			virtual ~EngineScope();
			engine_value_t getVariable(std::string);
			bool hasVariable(std::string);
			void putVariable(std::string, long double);
			bool hasFunction(std::string);
			engine_value_t evalFunction(std::string, std::vector<long double>&);
			bool addFunction(std::string, EngineFunction*);
		private:
			std::map<std::string, long double> vars;
			std::map<std::string, EngineFunction*> func;
	};
	
	class FunctionEngine {
		public:
			FunctionEngine();
			virtual ~FunctionEngine();
			EngineScope *getScope();
			engine_value_t eval(Node*);
		private:
			EngineScope scope;
	};
	
	void FunctionEngine_add_default_functions(FunctionEngine*);
}

#endif