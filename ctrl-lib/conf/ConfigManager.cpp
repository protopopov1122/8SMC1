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

#include "ctrl-lib/conf/ConfigManager.h"
#include "ctrl-lib/conf/ConfigValidator.h"
#include <algorithm>
#include <string.h>

namespace CalX {

	ConfigEntry::ConfigEntry(ConfigManager *conf, std::string name) {
		this->config = conf;
		this->name = name;
	}

	std::string ConfigEntry::getEntryName() {
		return this->name;
	}

	std::shared_ptr<ConfigValue> ConfigEntry::get(std::string id) {
		if (this->content.count(id) != 0) {
			return this->content[id];
		} else {
			return nullptr;
		}
	}

	bool ConfigEntry::has(std::string id) {
		return this->content.count(id) != 0;
	}

	bool ConfigEntry::put(std::string id, std::shared_ptr<ConfigValue> value) {
		bool change = false;
		if (this->has(id)) {
			this->content.erase(id);
			change = true;
		}
		if (value == nullptr) {
			return false;
		}
		this->content[id] = value;
		for (const auto &l : this->config->getEventListeners()) {
			if (change) {
				l->keyChanged(this->config, this, id);
			} else {
				l->keyAdded(this->config, this, id);
			}
		}
		return true;
	}

	bool ConfigEntry::remove(std::string id) {
		if (this->content.count(id) == 0) {
			return false;
		}
		this->content.erase(id);
		for (const auto &l : this->config->getEventListeners()) {
			l->keyRemoved(this->config, this, id);
		}
		return true;
	}

	bool ConfigEntry::is(std::string id, ConfigValueType type) {
		if (this->content.count(id) == 0) {
			return false;
		}
		std::shared_ptr<ConfigValue> val = this->content[id];
		return val->getType() == type;
	}

	int_conf_t ConfigEntry::getInt(std::string key, int_conf_t def) {
		if (this->is(key, ConfigValueType::Integer)) {
			return ((IntegerConfigValue *) this->get(key).get())->getValue();
		} else {
			return def;
		}
	}

	real_conf_t ConfigEntry::getReal(std::string key, real_conf_t def) {
		if (this->is(key, ConfigValueType::Real)) {
			return ((RealConfigValue *) this->get(key).get())->getValue();
		} else {
			return def;
		}
	}

	bool ConfigEntry::getBool(std::string key, bool def) {
		if (this->is(key, ConfigValueType::Boolean)) {
			return ((BoolConfigValue *) this->get(key).get())->getValue();
		} else {
			return def;
		}
	}

	std::string ConfigEntry::getString(std::string key, std::string def) {
		if (this->is(key, ConfigValueType::String)) {
			return ((StringConfigValue *) this->get(key).get())->getValue();
		} else {
			return def;
		}
	}

	void ConfigEntry::getContent(
	    std::vector<std::pair<std::string, std::shared_ptr<ConfigValue>>> &vec) {
		for (const auto &kv : this->content) {
			vec.push_back(make_pair(kv.first, kv.second));
		}
	}

	void ConfigEntry::store(std::ostream &os) {
		for (const auto &kv : this->content) {
			os << kv.first << '=';
			ConfigValue *value = kv.second.get();
			switch (value->getType()) {
				case ConfigValueType::Integer:
					os << ((IntegerConfigValue *) value)->getValue();
					break;
				case ConfigValueType::Real:
					os << ((RealConfigValue *) value)->getValue();
					break;
				case ConfigValueType::Boolean:
					os << (((BoolConfigValue *) value)->getValue() ? "true" : "false");
					break;
				case ConfigValueType::String:
					os << '\"' << ((StringConfigValue *) value)->getValue() << '\"';
					break;
			}
			os << std::endl;
		}
	}

	ConfigManager::ConfigManager() {
		this->validator = nullptr;
	}

	std::shared_ptr<ConfigEntry> ConfigManager::getEntry(std::string id,
	                                                     bool createNew) {
		if (this->entries.count(id) != 0) {
			return this->entries[id];
		} else if (createNew) {
			std::shared_ptr<ConfigEntry> entry =
			    std::make_shared<ConfigEntry>(this, id);
			this->entries[id] = entry;
			for (const auto &l : this->listeners) {
				l->entryAdded(this, id);
			}
			return entry;
		} else {
			return nullptr;
		}
	}

	bool ConfigManager::hasEntry(std::string id) {
		return this->entries.count(id) != 0;
	}

	bool ConfigManager::removeEntry(std::string id) {
		if (this->entries.count(id) == 0) {
			return false;
		}
		this->entries.erase(id);
		for (const auto &l : this->listeners) {
			l->entryRemoved(this, id);
		}
		return true;
	}

	void ConfigManager::getEntries(
	    std::vector<std::shared_ptr<ConfigEntry>> &vec) {
		for (const auto &kv : this->entries) {
			vec.push_back(kv.second);
		}
	}

	std::shared_ptr<ConfigValidator> ConfigManager::getValidator() {
		return this->validator;
	}

	void ConfigManager::setValidator(std::shared_ptr<ConfigValidator> v) {
		this->validator = v;
	}

	bool ConfigManager::validate(std::shared_ptr<ConfigValidator> v) {
		if (v == nullptr) {
			v = this->validator;
		}
		return v->validate(this);
	}

	void ConfigManager::store(std::ostream &os) {
		for (const auto &kv : this->entries) {
			os << '[' << kv.first << ']' << std::endl;
			kv.second->store(os);
			os << std::endl;
		}
	}

	void ConfigManager::addEventListener(std::shared_ptr<ConfigEventListener> l) {
		this->listeners.push_back(l);
	}

	void ConfigManager::removeEventListener(
	    std::shared_ptr<ConfigEventListener> l) {
		this->listeners.erase(
		    std::remove(this->listeners.begin(), this->listeners.end(), l),
		    this->listeners.end());
	}

	std::vector<std::shared_ptr<ConfigEventListener>>
	    &ConfigManager::getEventListeners() {
		return this->listeners;
	}

	size_t skipWhitespaces(const char *line, size_t start) {
		while (start < strlen(line) && isspace(line[start])) {
			start++;
		}
		return start;
	}

	std::unique_ptr<ConfigValue> ConfigManager::parseValue(const char *input) {
		size_t pos = 0;
		pos = skipWhitespaces(input, pos);
		const char *val = &input[pos];
		if (val[0] == '\"') {
			if (strlen(val) == 1 || val[strlen(val) - 1] != '\"') {
				//*err << "Error at line " << line_num <<  ": expected '\"' in the line"
				//<< std::endl;
				// continue;
				return nullptr;
			}
			std::string strval(&val[1]);
			strval.erase(strval.begin() + (std::ptrdiff_t) strval.length() - 1);
			return std::make_unique<StringConfigValue>(strval);
		} else if (strcmp(val, "true") == 0 || strcmp(val, "false") == 0) {
			bool boolval = strcmp(val, "true") == 0;
			return std::make_unique<BoolConfigValue>(boolval);
		} else {
			bool integer = true, real = false;
			for (size_t i = 0; i < strlen(val); i++) {
				if (val[i] == '.') {
					if (!real) {
						real = true;
					} else {
						integer = false;
						break;
					}
				} else if ((val[i] == '+' || val[i] == '-') && i == 0) {
				} else if (!isdigit(val[i])) {
					integer = false;
					break;
				}
			}
			if (integer) {
				if (real) {
					real_conf_t realval = std::stod(val);
					return std::make_unique<RealConfigValue>(realval);
				} else {
					int_conf_t intval = std::stoll(val);
					return std::make_unique<IntegerConfigValue>(intval);
				}
			} else {
				std::string strval(val);
				return std::make_unique<StringConfigValue>(strval);
			}
		}
	}

	std::unique_ptr<ConfigManager> ConfigManager::load(std::istream &is,
	                                                   std::ostream &err,
	                                                   ConfigManager *man) {
		std::unique_ptr<ConfigManager> new_man = nullptr;
		if (man == nullptr) {
			new_man = std::make_unique<ConfigManager>();
			man = new_man.get();
		}
		std::shared_ptr<ConfigEntry> entry = nullptr;
		const int LINE_LEN = 256;
		char rawline[LINE_LEN];
		int line_num = 0;
		while (is.good()) {
			is.getline(rawline, LINE_LEN);
			line_num++;
			// Remove comments
			for (size_t i = 0; i < strlen(rawline); i++) {
				if (rawline[i] == '#') {
					rawline[i] = '\0';
					break;
				}
			}
			// Trim input line
			size_t start = 0, end = strlen(rawline) - 1;
			while (start < strlen(rawline) && isspace(rawline[start])) {
				start++;
			}
			while (end < strlen(rawline) && end >= start && isspace(rawline[end])) {
				end--;
			}
			rawline[end + 1] = '\0';
			char *line = &rawline[start];
			if (strlen(line) == 0) {
				continue;
			}

			// Parse line
			if (line[0] == '[') {  // Line is entry start
				if (strlen(line) == 1 || line[strlen(line) - 1] != ']') {
					err << "Error at line " << line_num
					    << ": expected ']' at the end of line" << std::endl;
					continue;
				}
				line[strlen(line) - 1] = '\0';
				std::string entryId(&line[1]);
				entry = man->getEntry(entryId);
			} else {  // Line is a key/value pair
				if (entry == nullptr) {
					err << "Error at line " << line_num << ": entry not defined"
					    << std::endl;
				} else {
					std::string key;
					size_t pos = 0;
					while (
					    pos < strlen(line) &&
					    (isalpha(line[pos]) || isdigit(line[pos]) || line[pos] == '_')) {
						key.push_back(line[pos++]);
					}
					pos = skipWhitespaces(line, pos);
					if (pos >= strlen(line) || line[pos] != '=') {
						err << "Error at line " << line_num << ": expected '=' in the line"
						    << std::endl;
						continue;
					}
					pos = skipWhitespaces(line, ++pos);
					if (pos >= strlen(line)) {
						err << "Error at line " << line_num
						    << ": expected value in the line" << std::endl;
						continue;
					}
					std::unique_ptr<ConfigValue> value = parseValue(&line[pos]);
					if (value != nullptr) {
						entry->put(key, std::move(value));
					}
				}
			}
		}
		return new_man;
	}
}  // namespace CalX
