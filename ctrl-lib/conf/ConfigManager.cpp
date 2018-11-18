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

#include "ctrl-lib/conf/ConfigManager.h"
#include "ctrl-lib/conf/ConfigValidator.h"
#include <algorithm>
#include <iostream>

namespace CalX {

	ConfigManager::ConfigManager() {}

	ConfiguationFlatDictionary *ConfigManager::getEntry(const std::string &id,
	                                                     bool createNew) {
		if (this->entries.count(id) != 0) {
			return this->entries[id].get();
		} else if (createNew) {
			std::unique_ptr<ConfigEntry> entry =
			    std::make_unique<ConfigEntry>(*this, id);
			ConfigEntry *ptr = entry.get();
			this->entries[id] = std::move(entry);
			this->submitEvent(&ConfigEventListener::entryAdded, this, id);
			return ptr;
		} else {
			return nullptr;
		}
	}

	bool ConfigManager::hasEntry(const std::string &id) const {
		return this->entries.count(id) != 0;
	}

	bool ConfigManager::removeEntry(const std::string &id) {
		if (this->entries.count(id) == 0) {
			return false;
		}
		this->entries.erase(id);
		this->submitEvent(&ConfigEventListener::entryRemoved, this, id);
		return true;
	}

	void ConfigManager::visit(std::function<void (const std::string &, ConfiguationFlatDictionary &)> visit) const {
		for (const auto &kv : this->entries) {
			visit(kv.first, *kv.second);
		}
	}

	std::vector<std::shared_ptr<ConfigEventListener>>
	    &ConfigManager::getEventListeners() {
		return this->listeners;
	}
}  // namespace CalX
