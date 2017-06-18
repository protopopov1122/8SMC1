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


#ifndef CALX_CTRL_LIB_REQUEST_RESOLVER_H_
#define CALX_CTRL_LIB_REQUEST_RESOLVER_H_

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "CtrlCore.h"
#include "ConfigManager.h"

/* Request resolver - simple interface to perform system automatic configuration.
	Each configuration step should be written as request in format:
		provider:arg1;arg2;...;argN
	System searches requested provider and if found, provider is called with these arguments. */

namespace CalX {
	class SystemManager; // Forward referencing
	
	class Request {
		public:
			Request(std::string, std::vector<ConfigValue*>);
			virtual ~Request();
			std::string getProvider();
			std::vector<ConfigValue*> &getArguments();
		private:
			std::string provider;
			std::vector<ConfigValue*> args;
	};
	
	class RequestProvider {
		public:
			RequestProvider(std::string);
			virtual ~RequestProvider();
			std::string getName();
			virtual bool execute(Request*, SystemManager*) = 0;
		private:
			std::string name;
	};
	
	class RequestResolveWatcher {
		public:
			RequestResolveWatcher() {}
			virtual ~RequestResolveWatcher() {}
			virtual void resolving(std::string, size_t, size_t) {}
			virtual void failed(std::string, size_t, size_t) {}
	};
	
	class RequestResolver {
		public:
			RequestResolver(SystemManager*);
			virtual ~RequestResolver();
			
			bool registerProvider(RequestProvider*);
			bool resolveRequest(Request*);
			bool hasProvider(std::string);
			void execute(std::istream*, RequestResolveWatcher* = nullptr);
			
			static Request *parseRequest(std::string);
		private:
			SystemManager *sysman;
			std::map<std::string, RequestProvider*> providers;
	};
	
	void RequestResolver_init_standart_providers(RequestResolver*);
	
	#define PROVIDER_ARGC(req, count)\
		if (req->getArguments().size() != count) {\
			return false;\
		}
	#define PROVIDER_ARG_TYPE(req, index, type)\
		if (index >= req->getArguments().size() ||\
			req->getArguments().at(index)->getType() != type) {\
			return false;\
		}
	#define PROVIDER_ARG(req, index) (req->getArguments().at(index))
}

#endif