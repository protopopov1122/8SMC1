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


#include "RequestResolver.h"
#include "SystemManager.h"

/* This file contains some standart request provides */

namespace CalX {
	#define PROVIDER(clname, name)\
		class clname : public RequestProvider {\
			public:\
				clname() : RequestProvider::RequestProvider(name) {}\
				~clname() {}\
				virtual bool execute(Request*, SystemManager*);\
		};\
		bool clname::execute(Request *req, SystemManager *sysman)
		
	
	PROVIDER(EchoProvider, "echo") {
		std::vector<ConfigValue*> &args = req->getArguments();
		for (const auto& arg : args) {
			std::cout << arg->toString() << " ";
		}
		std::cout << std::endl;
		return true;
	}
	
	PROVIDER(NullProvider, "") {
		return true;
	}

	void RequestResolver_init_standart_providers(RequestResolver *res) {
		#define REG(clname) res->registerProvider(new clname())
		REG(EchoProvider);
		REG(NullProvider);
	}
}