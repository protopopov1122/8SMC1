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
#include <string.h>
#include <iostream>

namespace CalX {

	ConfigManager::ConfigManager() {
		this->validator = nullptr;
	}

	ConfigEntry *ConfigManager::getEntry(std::string id,
	                                                     bool createNew) {
		if (this->entries.count(id) != 0) {
			return this->entries[id].get();
		} else if (createNew) {
			std::unique_ptr<ConfigEntry> entry =
			    std::make_unique<ConfigEntry>(*this, id);
			ConfigEntry *entry_ptr = entry.get();
			this->entries[id] = std::move(entry);
			for (const auto &l : this->listeners) {
				l->entryAdded(this, id);
			}
			return entry_ptr;
		} else {
			return nullptr;
		}
	}

	bool ConfigManager::hasEntry(std::string id) const {
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
	    std::vector<ConfigEntry *> &vec) const {
		for (const auto &kv : this->entries) {
			vec.push_back(kv.second.get());
		}
	}

	std::shared_ptr<ConfigValidator> ConfigManager::getValidator() const {
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

	void ConfigManager::store(std::ostream &os) const {
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

	ConfigurationValue ConfigManager::parseValue(const char *input) {
		size_t pos = 0;
		pos = skipWhitespaces(input, pos);
		const char *val = &input[pos];
		if (val[0] == '\"') {
			if (strlen(val) == 1 || val[strlen(val) - 1] != '\"') {
				//*err << "Error at line " << line_num <<  ": expected '\"' in the line"
				//<< std::endl;
				// continue;
				return ConfigurationValue::Empty;
			}
			std::string strval(&val[1]);
			strval.erase(strval.begin() + (std::ptrdiff_t) strval.length() - 1);
			return ConfigurationValue(strval);
		} else if (strcmp(val, "true") == 0 || strcmp(val, "false") == 0) {
			bool boolval = strcmp(val, "true") == 0;
			return ConfigurationValue(boolval);
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
					return ConfigurationValue(realval);
				} else {
					int_conf_t intval = std::stoll(val);
					return ConfigurationValue(intval);
				}
			} else {
				std::string strval(val);
				return ConfigurationValue(strval);
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
		ConfigEntry *entry = nullptr;
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
					ConfigurationValue value = parseValue(&line[pos]);
					if (!value.is(ConfigValueType::None)) {
						entry->put(key, std::move(value));
					}
				}
			}
		}
		return new_man;
	}
}  // namespace CalX
