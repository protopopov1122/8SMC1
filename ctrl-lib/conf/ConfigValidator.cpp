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

#include "ctrl-lib/conf/ConfigValidator.h"

namespace CalX {

	AbstractValidator::AbstractValidator(bool opt) : optional(opt) {}

	bool AbstractValidator::isOptional() {
		return this->optional;
	}

	ConfigValueValidator::ConfigValueValidator(ConfigValueType tp, bool opt)
	    : AbstractValidator::AbstractValidator(opt), type(tp) {}

	ConfigValueType ConfigValueValidator::getType() {
		return this->type;
	}

	bool ConfigValueValidator::validate(std::shared_ptr<ConfigValue> value) {
		if (value == nullptr) {
			return this->isOptional();
		}
		return value->getType() == type || this->isOptional();
	}

	ConfigKeyValidator::ConfigKeyValidator(
	    std::string k, std::unique_ptr<ConfigValueValidator> v, bool opt)
	    : AbstractValidator::AbstractValidator(opt),
	      key(k),
	      value(std::move(v)) {}

	std::string ConfigKeyValidator::getKey() {
		return this->key;
	}

	ConfigValueValidator *ConfigKeyValidator::getValue() {
		return this->value.get();
	}

	bool ConfigKeyValidator::validate(std::shared_ptr<ConfigEntry> entry) {
		if (entry == nullptr || !entry->has(key)) {
			return this->isOptional();
		}
		std::shared_ptr<ConfigValue> value = entry->get(key);
		return this->value->validate(value);
	}

	ConfigEntryValidator::ConfigEntryValidator(std::string nm, bool opt)
	    : AbstractValidator::AbstractValidator(opt), name(nm) {}

	std::string ConfigEntryValidator::getEntryName() {
		return this->name;
	}

	void ConfigEntryValidator::addKeyValidator(
	    std::unique_ptr<ConfigKeyValidator> kv) {
		this->keys.push_back(std::move(kv));
	}

	bool ConfigEntryValidator::validate(ConfigManager *conf) {
		if (conf == nullptr || !conf->hasEntry(this->name)) {
			return this->isOptional();
		}

		std::shared_ptr<ConfigEntry> entry = conf->getEntry(this->name, false);
		for (const auto &k : this->keys) {
			if (!k->validate(entry)) {
				return false;
			}
		}
		return true;
	}

	ConfigValidator::ConfigValidator(bool opt)
	    : AbstractValidator::AbstractValidator(opt) {}

	void ConfigValidator::addEntryValidator(
	    std::unique_ptr<ConfigEntryValidator> entry) {
		this->entries.push_back(std::move(entry));
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
}  // namespace CalX
