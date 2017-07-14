/*
        Copyright (c) 2017 Jevgenijs Protopopovs

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

#include "ctrl-lib/conf/ConfigValidator.h"

namespace CalX {

	AbstractValidator::AbstractValidator(bool opt) {
		this->optional = opt;
	}

	AbstractValidator::~AbstractValidator() {}

	bool AbstractValidator::isOptional() {
		return this->optional;
	}

	ConfigValueValidator::ConfigValueValidator(ConfigValueType type, bool opt)
	    : AbstractValidator::AbstractValidator(opt) {
		this->type = type;
	}

	ConfigValueValidator::~ConfigValueValidator() {}

	ConfigValueType ConfigValueValidator::getType() {
		return this->type;
	}

	bool ConfigValueValidator::validate(ConfigValue *value) {
		if (value == nullptr) {
			return this->isOptional();
		}
		return value->getType() == type || this->isOptional();
	}

	ConfigKeyValidator::ConfigKeyValidator(std::string key,
	                                       ConfigValueValidator *value, bool opt)
	    : AbstractValidator::AbstractValidator(opt) {
		this->key = key;
		this->value = value;
	}

	ConfigKeyValidator::~ConfigKeyValidator() {
		delete this->value;
	}

	std::string ConfigKeyValidator::getKey() {
		return this->key;
	}

	ConfigValueValidator *ConfigKeyValidator::getValue() {
		return this->value;
	}

	bool ConfigKeyValidator::validate(ConfigEntry *entry) {
		if (entry == nullptr || !entry->has(key)) {
			return this->isOptional();
		}
		ConfigValue *value = entry->get(key);
		return this->value->validate(value);
	}

	ConfigEntryValidator::ConfigEntryValidator(std::string name, bool opt)
	    : AbstractValidator::AbstractValidator(opt) {
		this->name = name;
	}

	ConfigEntryValidator::~ConfigEntryValidator() {
		for (const auto &k : this->keys) {
			delete k;
		}
	}

	std::string ConfigEntryValidator::getEntryName() {
		return this->name;
	}

	void ConfigEntryValidator::addKeyValidator(ConfigKeyValidator *kv) {
		this->keys.push_back(kv);
	}

	bool ConfigEntryValidator::validate(ConfigManager *conf) {
		if (conf == nullptr || !conf->hasEntry(this->name)) {
			return this->isOptional();
		}

		ConfigEntry *entry = conf->getEntry(this->name, false);
		for (const auto &k : this->keys) {
			if (!k->validate(entry)) {
				return false;
			}
		}
		return true;
	}

	ConfigValidator::ConfigValidator(bool opt)
	    : AbstractValidator::AbstractValidator(opt) {}

	ConfigValidator::~ConfigValidator() {
		for (const auto &entry : this->entries) {
			delete entry;
		}
	}

	void ConfigValidator::addEntryValidator(ConfigEntryValidator *entry) {
		this->entries.push_back(entry);
	}

	bool ConfigValidator::validate(ConfigManager *conf) {
		if (conf == nullptr) {
			return this->isOptional();
		}
		for (const auto &entry : this->entries) {
			if (!entry->validate(conf)) {
				return false;
			}
		}
		return true;
	}
}
