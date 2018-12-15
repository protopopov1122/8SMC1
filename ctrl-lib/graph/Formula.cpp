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

#include "ctrl-lib/graph/Formula.h"
#include "ctrl-lib/graph/FunctionParser.h"
#include <sstream>

namespace CalX {

	class MathFormulaScope : public EngineScope {
	 public:
		MathFormulaScope(EngineScope &base) : baseScope(base) {}
		engine_value_t getVariable(std::string key) const override {
			if (this->scope.hasVariable(key)) {
				return this->scope.getVariable(key);
			} else {
				return this->baseScope.getVariable(key);
			}
		}

		bool hasVariable(std::string key) const override {
			return this->scope.hasVariable(key) || this->baseScope.hasVariable(key);
		}

		void putVariable(std::string key, double value) override {
			this->scope.putVariable(key, value);
		}

		bool hasFunction(std::string key) const override {
			return this->scope.hasFunction(key) || this->baseScope.hasFunction(key);
		}

		engine_value_t evalFunction(std::string key,
		                            std::vector<double> &args) const override {
			if (this->scope.hasFunction(key)) {
				return this->scope.evalFunction(key, args);
			} else {
				return this->baseScope.evalFunction(key, args);
			}
		}

		bool addFunction(std::string name,
		                 std::unique_ptr<EngineFunction> fn) override {
			return this->scope.addFunction(name, std::move(fn));
		}

	 private:
		EngineScope &baseScope;
		MapEngineScope scope;
	};

	class MathFormulaEngine : public MathEngine {
	 public:
		MathFormulaEngine(MathEngine &baseEngine) : scope(baseEngine.getScope()) {}

		EngineScope &getScope() override {
			return this->scope;
		}

		engine_value_t eval(Node &node) override {
			return node.eval(*this);
		}

	 private:
		MathFormulaScope scope;
	};

	std::unique_ptr<MathFormula> MathFormula::build(
	    const std::string &formula, const std::vector<std::string> &variables) {
		std::stringstream ss(formula);
		FunctionParser parser(std::make_unique<FunctionLexer>(ss));
		return std::make_unique<MathBaseFormula>(parser.parse(), variables);
	}

	MathBaseFormula::MathBaseFormula(std::unique_ptr<Node> formula,
	                                 const std::vector<std::string> &variables)
	    : formula(std::move(formula)), variables(variables) {}

	const std::vector<std::string> &MathBaseFormula::getVariables() const {
		return this->variables;
	}

	engine_value_t MathBaseFormula::eval(
	    MathEngine &baseEngine,
	    const std::map<std::string, double> &variableValues) {
		MathFormulaEngine engine(baseEngine);
		for (const std::string &key : this->variables) {
			if (variableValues.count(key) == 0) {
				return engine_value_t{ 0, MathError::MNoVariable };
			}
			engine.getScope().putVariable(key, variableValues.at(key));
		}
		return engine.eval(*this->formula);
	}
}  // namespace CalX