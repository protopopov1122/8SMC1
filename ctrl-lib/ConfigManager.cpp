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


#include <string.h>
#include "ConfigManager.h"

namespace CalX {
	
	ConfigEntry::ConfigEntry(std::string name) {
		this->name = name;
	}
	
	ConfigEntry::~ConfigEntry() {
		for (const auto& kv : this->content) {
			delete kv.second;
		}
		this->content.clear();
	}
	
	std::string ConfigEntry::getEntryName() {
		return this->name;
	}
	
	ConfigValue *ConfigEntry::get(std::string id) {
		if (this->content.count(id) != 0) {
			return this->content[id];
		} else {
			return nullptr;
		}
	}
	
	bool ConfigEntry::has(std::string id) {
		return this->content.count(id) != 0;
	}
	
	bool ConfigEntry::put(std::string id, ConfigValue *value) {
		if (this->has(id)) {
			this->remove(id);
		}
		if (value == nullptr) {
			return false;
		}
		this->content[id] = value;
		return true;
	}
	
	bool ConfigEntry::remove(std::string id) {
		if (this->content.count(id) == 0) {
			return false;
		}
		delete this->content[id];
		this->content.erase(id);
		return true;
	}
	
	bool ConfigEntry::is(std::string id, ConfigValueType type) {
		if (this->content.count(id) == 0) {
			return false;
		}
		ConfigValue *val = this->content[id];
		return val->getType() == type;
	}
	
	ConfigManager::ConfigManager() {
		
	}
	
	ConfigManager::~ConfigManager() {
		for (const auto& kv : this->entries) {
			delete kv.second;
		}
		this->entries.clear();
	}
	
	ConfigEntry *ConfigManager::getEntry(std::string id, bool createNew) {
		if (this->entries.count(id) != 0) {
			return this->entries[id];
		} else if (createNew) {
			ConfigEntry *entry = new ConfigEntry(id);
			this->entries[id] = entry;
			return entry;
		} else {
			return nullptr;
		}
	}
	
	bool ConfigManager::hasEntry(std::string id) {
		return this->entries.count(id) != 0;
	}
	
	ConfigManager *ConfigManager::load(std::istream *is) {
		ConfigManager *man = new ConfigManager();
		// TODO Write file parser
		return man;
	}
}

