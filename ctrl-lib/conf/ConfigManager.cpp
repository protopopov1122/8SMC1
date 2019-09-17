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

#include "calx/ctrl-lib/conf/ConfigManager.h"
#include "calx/ctrl-lib/conf/ConfigValidator.h"
#include <algorithm>

namespace CalX {

	class ConfigManagerDictionaryListener : public FlatDictionaryListener {
	 public:
		ConfigManagerDictionaryListener(
		    ConfigurationCatalogue &cat, const std::string &entry,
		    const std::vector<std::shared_ptr<CatalogueListener>> &listeners)
		    : catalogue(cat), entry(entry), listeners(listeners) {}

		void keyAdd(ConfiguationFlatDictionary &dist,
		            const std::string &key) override {
			for (auto &l : this->listeners) {
				l->keyAdd(&this->catalogue, this->entry, key);
			}
		}

		void keyChange(ConfiguationFlatDictionary &dist,
		               const std::string &key) override {
			for (auto &l : this->listeners) {
				l->keyChange(&this->catalogue, this->entry, key);
			}
		}

		void keyRemove(ConfiguationFlatDictionary &dist,
		               const std::string &key) override {
			for (auto &l : this->listeners) {
				l->keyRemove(&this->catalogue, this->entry, key);
			}
		}

	 private:
		ConfigurationCatalogue &catalogue;
		std::string entry;
		const std::vector<std::shared_ptr<CatalogueListener>> &listeners;
	};

	ConfigManager::ConfigManager() {}

	ConfiguationFlatDictionary *ConfigManager::getEntry(const std::string &id,
	                                                    bool createNew) {
		if (this->entries.count(id) != 0) {
			return this->entries[id].get();
		} else if (createNew) {
			std::unique_ptr<ConfigEntry> entry = std::make_unique<ConfigEntry>();
			entry->addEventListener(std::make_shared<ConfigManagerDictionaryListener>(
			    *this, id, this->listeners));
			ConfigEntry *ptr = entry.get();
			this->entries[id] = std::move(entry);
			this->submitEvent(&CatalogueListener::entryAdd, this, id);
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
		this->submitEvent(&CatalogueListener::entryRemove, this, id);
		this->entries.erase(id);
		return true;
	}

	void ConfigManager::visit(
	    std::function<void(const std::string &, ConfiguationFlatDictionary &)>
	        visit) const {
		for (const auto &kv : this->entries) {
			visit(kv.first, *kv.second);
		}
	}
}  // namespace CalX
