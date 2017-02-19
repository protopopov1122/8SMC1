#ifndef _8SMC1_CTRL_LIB_GRAPH_FUNCTION_ENGINE_H_
#define _8SMC1_CTRL_LIB_GRAPH_FUNCTION_ENGINE_H_

#include <map>
#include <vector>
#include <string>
#include <cinttypes>
#include "AST.h"

namespace _8SMC1 {
	
	struct coord_point_t {
		long double x;
		long double y;
	};
	
	struct coord_size_t {
		long double w;
		long double h;
	};
		
	class EngineFunction {
		public:
			virtual ~EngineFunction() {}
			virtual engine_value_t eval(std::vector<engine_value_t>&) = 0;
	};
	
	class EngineScope {
		public:
			EngineScope();
			virtual ~EngineScope();
			engine_value_t getVariable(std::string);
			bool hasVariable(std::string);
			void putVariable(std::string, engine_value_t);
			bool hasFunction(std::string);
			engine_value_t evalFunction(std::string, std::vector<engine_value_t>&);
			bool addFunction(std::string, EngineFunction*);
		private:
			std::map<std::string, engine_value_t> vars;
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
}

#endif