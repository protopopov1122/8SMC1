#include "FunctionEngine.h"
#include <math.h>

/* This file contains implementation of some mathematical functions*
   that can be used with math evaluator.*/

namespace _8SMC1 {
	
#define DefFun1(id, fn)\
	class id : public EngineFunction {\
		public:\
			id() {}\
			virtual ~id() {}\
			virtual engine_value_t eval(std::vector<long double> &args) {\
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