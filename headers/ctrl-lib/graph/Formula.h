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

#ifndef CALX_CTRL_LIB_GRAPH_FORMULA_H_
#define CALX_CTRL_LIB_GRAPH_FORMULA_H_

#include "ctrl-lib/graph/MathEngine.h"
#include <map>

namespace CalX {

	class MathFormula {
	 public:
		virtual ~MathFormula() = default;
		virtual const std::vector<std::string> &getVariables() const = 0;
		virtual engine_value_t eval(MathEngine &,
		                            const std::map<std::string, double> &) = 0;

		static std::unique_ptr<MathFormula> build(const std::string &,
		                                          const std::vector<std::string> &);
	};

	class MathBaseFormula : public MathFormula {
	 public:
		MathBaseFormula(std::unique_ptr<Node>, const std::vector<std::string> &);

		const std::vector<std::string> &getVariables() const override;
		engine_value_t eval(MathEngine &,
		                    const std::map<std::string, double> &) override;

	 private:
		std::unique_ptr<Node> formula;
		std::vector<std::string> variables;
	};
}  // namespace CalX

#endif