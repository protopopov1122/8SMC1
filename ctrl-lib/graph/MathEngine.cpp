/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calx/ctrl-lib/graph/MathEngine.h"

namespace CalX {

	engine_value_t MapEngineScope::getVariable(const std::string &id) const {
		engine_value_t val = { 0, MathError::MNoError };
		if (this->vars.count(id) == 0) {
			val.err = MathError::MNoVariable;
			return val;
		} else {
			val.value = this->vars.at(id);
			return val;
		}
	}

	bool MapEngineScope::hasVariable(const std::string &id) const {
		return this->vars.count(id) != 0;
	}

	void MapEngineScope::putVariable(const std::string &id, double val) {
		this->vars[id] = val;
	}

	bool MapEngineScope::hasFunction(const std::string &id) const {
		return this->func.count(id) != 0;
	}

	bool MapEngineScope::addFunction(const std::string &id,
	                                 std::unique_ptr<EngineFunction> func) {
		if (this->func.count(id) != 0) {
			return false;
		}
		this->func[id] = std::move(func);
		return true;
	}

	engine_value_t MapEngineScope::evalFunction(const std::string &id,
	                                            std::vector<double> &args) const {
		if (this->func.count(id) == 0) {
			engine_value_t val = { 0, MathError::MNoFunction };
			return val;
		} else {
			return this->func.at(id)->eval(args);
		}
	}

	DefaultMathEngine::DefaultMathEngine() {
		MathEngine_add_default_functions(*this);
	}

	EngineScope &DefaultMathEngine::getScope() {
		return this->scope;
	}

	engine_value_t DefaultMathEngine::eval(Node &node) {
		return node.eval(*this);
	}
}  // namespace CalX
