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

#include <string>
#include <string.h>
#include "ctrl-lib/conf/RequestResolver.h"
#include "ctrl-lib/SystemManager.h"

namespace CalX {

  Request::Request(std::string provider, std::vector<ConfigValue *> args) {
	this->provider = provider;
	this->args = args;
  }

  Request::~Request() {
	for (const auto &arg : this->args) {
	  delete arg;
	}
	this->args.clear();
  }

  std::string Request::getProvider() {
	return this->provider;
  }

  std::vector<ConfigValue *> &Request::getArguments() {
	return this->args;
  }

  RequestProvider::RequestProvider(std::string name) {
	this->name = name;
  }

  RequestProvider::~RequestProvider() {}

  std::string RequestProvider::getName() {
	return this->name;
  }

  RequestResolver::RequestResolver(SystemManager *sysman) {
	this->sysman = sysman;
  }

  RequestResolver::~RequestResolver() {
	for (const auto &kv : this->providers) {
	  delete kv.second;
	}
  }

  bool RequestResolver::registerProvider(RequestProvider *p) {
	if (p == nullptr || this->providers.count(p->getName()) != 0) {
	  return false;
	}
	this->providers[p->getName()] = p;
	return true;
  }

  bool RequestResolver::resolveRequest(Request *req) {
	if (req == nullptr || this->providers.count(req->getProvider()) == 0) {
	  return false;
	}
	RequestProvider *prov = this->providers[req->getProvider()];
	return prov->execute(req, sysman);
  }

  bool RequestResolver::hasProvider(std::string name) {
	return this->providers.count(name) != 0;
  }

  void trimString(std::string &str) {
	for (size_t i = str.length() - 1; i < str.length(); i--) {
	  if (isspace(str.at(i))) {
		str.erase(str.begin() + (std::ptrdiff_t) i);
	  } else {
		break;
	  }
	}
  }

  Request *RequestResolver::parseRequest(std::string str) {
	const char *cstr = str.c_str();
	/*for (size_t i = 0; i < strlen(cstr); i++) {
		if (cstr[i] == '#') {
			cstr[i] = '\0';
			break;
		}
	}
	std::string provider;
	std::vector<ConfigValue*> args;
	while (*cstr != ':' &&
		*cstr != '\0') {
		provider.push_back(*cstr);
		cstr++;
	}
	if (*cstr == ':') {
		cstr++;
	}
	std::string arg;
	while (*cstr != '\0') {
		if (*cstr == ',') {
			trimString(arg);
			if (!arg.empty()) {
				ConfigValue *value = ConfigManager::parseValue(arg.c_str());
				if (value != nullptr) {
					args.push_back(value);
				}
				arg.clear();
			}
		} else {
			arg.push_back(*cstr);
		}
		cstr++;
	}
		trimString(arg);
	if (!arg.empty()) {
		ConfigValue *value = ConfigManager::parseValue(arg.c_str());
		if (value != nullptr) {
			args.push_back(value);
		}
		arg.clear();
	}
	delete[] cstr;
	if (provider.empty() && args.empty()) {
		return nullptr;
	}
	return new Request(provider, args);*/

	std::string provider;
	std::vector<ConfigValue *> args;
	size_t pos = 0;
	for (; pos < strlen(cstr) && cstr[pos] != ':' && cstr[pos] != '#'; pos++) {
	  if (isspace(cstr[pos])) {
		continue;
	  }
	  provider.push_back(cstr[pos]);
	}
	if (cstr[pos] == ':') {
	  pos++;
	}

	bool quote = false;
	std::string arg;
	for (; pos < strlen(cstr); pos++) {
	  char chr = cstr[pos];
	  if (chr == ',' && !quote) {
		if (!arg.empty()) {
		  ConfigValue *value = ConfigManager::parseValue(arg.c_str());
		  if (value != nullptr) {
			args.push_back(value);
		  }
		  arg.clear();
		}
	  } else if (isspace(chr) && !quote) {
	  } else if (chr == '#' && !quote) {
		break;
	  } else {
		if (chr == '\"') {
		  quote = !quote;
		}
		arg.push_back(chr);
	  }
	}
	if (!arg.empty()) {
	  ConfigValue *value = ConfigManager::parseValue(arg.c_str());
	  if (value != nullptr) {
		args.push_back(value);
	  }
	}
	if (provider.empty() && args.empty()) {
	  return nullptr;
	}

	return new Request(provider, args);
  }

  void RequestResolver::execute(std::istream *is,
								RequestResolveWatcher *watcher) {
	const size_t LINE_LEN = 1024;
	char line[LINE_LEN];
	std::vector<Request *> init_code;
	std::vector<std::string> code;
	while (!is->eof()) {
	  is->getline(line, LINE_LEN);
	  Request *req = RequestResolver::parseRequest(std::string(line));
	  if (req != nullptr) {
		code.push_back(std::string(line));
		init_code.push_back(req);
	  }
	}

	for (size_t i = 0; i < init_code.size(); i++) {
	  if (watcher != nullptr) {
		watcher->resolving(code.at(i), i + 1, init_code.size());
	  }
	  if (!this->resolveRequest(init_code.at(i)) && watcher != nullptr) {
		watcher->failed(code.at(i), i + 1, init_code.size());
		break;
	  }
	}

	for (const auto &req : init_code) {
	  delete req;
	}
  }
}
