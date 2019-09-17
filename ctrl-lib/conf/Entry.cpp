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

namespace CalX {

	ConfigEntry::ConfigEntry() {}

	const ConfigurationValue &ConfigEntry::get(const std::string &id) const {
		if (this->content.count(id) != 0) {
			return this->content.at(id);
		} else {
			return ConfigurationValue::Empty;
		}
	}

	bool ConfigEntry::has(const std::string &id) const {
		return this->content.count(id) != 0;
	}

	bool ConfigEntry::put(const std::string &id,
	                      const ConfigurationValue &value) {
		bool change = false;
		if (this->has(id)) {
			this->content.erase(id);
			change = true;
		}
		if (value.is(ConfigurationValueType::None)) {
			return false;
		}
		this->content[id] = value;
		if (change) {
			this->submitEvent(&FlatDictionaryListener::keyChange, *this, id);
		} else {
			this->submitEvent(&FlatDictionaryListener::keyAdd, *this, id);
		}
		return true;
	}

	bool ConfigEntry::remove(const std::string &id) {
		if (this->content.count(id) == 0) {
			return false;
		}
		this->content.erase(id);
		this->submitEvent(&FlatDictionaryListener::keyRemove, *this, id);
		return true;
	}

	void ConfigEntry::visit(
	    std::function<void(const std::string &, const ConfigurationValue &)>
	        visit) const {
		for (const auto &kv : this->content) {
			visit(kv.first, kv.second);
		}
	}
}  // namespace CalX