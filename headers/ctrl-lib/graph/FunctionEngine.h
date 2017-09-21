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

#ifndef CALX_CTRL_LIB_GRAPH_FUNCTION_ENGINE_H_
#define CALX_CTRL_LIB_GRAPH_FUNCTION_ENGINE_H_

#include "ctrl-lib/CtrlCore.h"
#include "ctrl-lib/graph/AST.h"
#include <cinttypes>
#include <memory>
#include <map>
#include <string>
#include <vector>

/* This file contains definition of function evaluation engine and scope.
   Scope stores variable values and defined mathematical functions.
   It evaluate AST nodes and provide them values and functions.*/

namespace CalX {

	class EngineFunction {
	 public:
		virtual ~EngineFunction() = default;
		virtual engine_value_t eval(std::vector<double> &) = 0;
	};

	class EngineScope {
	 public:
		EngineScope();
		engine_value_t getVariable(std::string);
		bool hasVariable(std::string);
		void putVariable(std::string, double);
		bool hasFunction(std::string);
		engine_value_t evalFunction(std::string, std::vector<double> &);
		bool addFunction(std::string, std::unique_ptr<EngineFunction>);

	 private:
		std::map<std::string, double> vars;
		std::map<std::string, std::unique_ptr<EngineFunction>> func;
	};

	class FunctionEngine {
	 public:
		FunctionEngine();
		virtual ~FunctionEngine() = default;
		EngineScope &getScope();
		engine_value_t eval(Node *);

	 private:
		EngineScope scope;
	};

	void FunctionEngine_add_default_functions(FunctionEngine *);
}  // namespace CalX

#endif