/*
    Copyright (c) 2017 Jevgenijs Protopopovs

    This file is part of CalX project.

    CalX is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CalX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "FunctionEngine.h"
#include <math.h>

/* This file contains implementation of some mathematical functions*
   that can be used with math evaluator.*/

namespace CalX {
	
#define DefFun1(id, fn)\
	class id : public EngineFunction {\
		public:\
			id() {}\
			virtual ~id() {}\
			virtual engine_value_t eval(std::vector<double> &args) {\
				engine_value_t res = {0, MathError::MNoError};\
				if (args.size() != 1){\
					res.err = MathError::MWrongParameters;\
				}\
				res.value = fn(args.at(0));\
				return res;\
			}\
	};
	
	DefFun1(SinFunction, sin)
	DefFun1(CosFunction, cos)
	DefFun1(TanFunction, tan)
	DefFun1(AsinFunction, asin)
	DefFun1(AcosFunction, acos)
	DefFun1(AtanFunction, atan)
	DefFun1(ExpFunction, exp)
	DefFun1(LogFunction, log)
	DefFun1(Log10Function, log10)
	DefFun1(Log2Function, log2)
	DefFun1(SqrtFunction, sqrt)
	DefFun1(CbrtFunction, cbrt)
	DefFun1(CeilFunction, ceil)
	DefFun1(FloorFunction, floor)
	DefFun1(FabsFunction, fabs)

	void FunctionEngine_add_default_functions(FunctionEngine *engine) {
		engine->getScope()->addFunction("sin", new SinFunction());
		engine->getScope()->addFunction("cos", new CosFunction());
		engine->getScope()->addFunction("tan", new TanFunction());
		engine->getScope()->addFunction("asin", new AsinFunction());
		engine->getScope()->addFunction("acos", new AcosFunction());
		engine->getScope()->addFunction("atan", new AtanFunction());
		engine->getScope()->addFunction("exp", new ExpFunction());
		engine->getScope()->addFunction("log", new LogFunction());
		engine->getScope()->addFunction("log10", new Log10Function());
		engine->getScope()->addFunction("log2", new Log2Function());
		engine->getScope()->addFunction("sqrt", new SqrtFunction());
		engine->getScope()->addFunction("cbrt", new CbrtFunction());
		engine->getScope()->addFunction("ceil", new CeilFunction());
		engine->getScope()->addFunction("floor", new FloorFunction());
		engine->getScope()->addFunction("abs", new FabsFunction());
	}

}