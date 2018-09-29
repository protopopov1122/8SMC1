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

#include "ctrl-lib/graph/FunctionEngine.h"

namespace CalX {

	EngineScope::EngineScope() {}

	engine_value_t EngineScope::getVariable(std::string id) {
		engine_value_t val = { 0, MathError::MNoError };
		if (this->vars.count(id) == 0) {
			val.err = MathError::MNoVariable;
			return val;
		}
		val.value = this->vars[id];
		return val;
	}

	bool EngineScope::hasVariable(std::string id) const {
		return this->vars.count(id) != 0;
	}

	void EngineScope::putVariable(std::string id, double val) {
		this->vars[id] = val;
	}

	bool EngineScope::hasFunction(std::string id) const {
		return this->func.count(id) != 0;
	}

	bool EngineScope::addFunction(std::string id,
	                              std::unique_ptr<EngineFunction> func) {
		if (this->func.count(id) != 0) {
			return false;
		}
		this->func[id] = std::move(func);
		return true;
	}

	engine_value_t EngineScope::evalFunction(std::string id,
	                                         std::vector<double> &args) {
		if (this->func.count(id) == 0) {
			engine_value_t val = { 0, MathError::MNoFunction };
			return val;
		}
		return this->func[id]->eval(args);
	}

	FunctionEngine::FunctionEngine() {}

	EngineScope &FunctionEngine::getScope() {
		return this->scope;
	}

	engine_value_t FunctionEngine::eval(Node &node) {
		return node.eval(*this);
	}
}  // namespace CalX
