/*
        Copyright (c) 2017-2018 Jevgenijs Protopopovs

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

#ifndef CALX_CTRL_LIB_GRAPH_MATH_ENGINE_H_
#define CALX_CTRL_LIB_GRAPH_MATH_ENGINE_H_

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
		virtual ~EngineScope() = default;
		virtual engine_value_t getVariable(std::string) const = 0;
		virtual bool hasVariable(std::string) const = 0;
		virtual void putVariable(std::string, double) = 0;
		virtual bool hasFunction(std::string) const = 0;
		virtual engine_value_t evalFunction(std::string,
		                                    std::vector<double> &) const = 0;
		virtual bool addFunction(std::string, std::unique_ptr<EngineFunction>) = 0;
	};

	class MathEngine {
	 public:
		virtual ~MathEngine() = default;
		virtual EngineScope &getScope() = 0;
		virtual engine_value_t eval(Node &) = 0;
	};

	class MapEngineScope : public EngineScope {
	 public:
		engine_value_t getVariable(std::string) const override;
		bool hasVariable(std::string) const override;
		void putVariable(std::string, double) override;
		bool hasFunction(std::string) const override;
		engine_value_t evalFunction(std::string,
		                            std::vector<double> &) const override;
		bool addFunction(std::string, std::unique_ptr<EngineFunction>) override;

	 private:
		std::map<std::string, double> vars;
		std::map<std::string, std::unique_ptr<EngineFunction>> func;
	};

	class DefaultMathEngine : public MathEngine {
	 public:
		DefaultMathEngine();
		EngineScope &getScope() override;
		engine_value_t eval(Node &) override;

	 private:
		MapEngineScope scope;
	};

	void MathEngine_add_default_functions(MathEngine &);
}  // namespace CalX

#endif