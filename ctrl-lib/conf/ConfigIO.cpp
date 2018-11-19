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
#include <iostream>
#include <string.h>

namespace CalX {

  void ConfigManagerIO::store(ConfigManager &config, std::ostream &os) {
    config.visit([&](const std::string &entryName, ConfiguationFlatDictionary &entry) {
      os << '[' << entryName << ']' << std::endl;
			ConfigManagerIO::store(entry, os);
			os << std::endl;
    });
  }

  void ConfigManagerIO::store(ConfiguationFlatDictionary &entry, std::ostream &os) {
    entry.visit([&](const std::string &key, const ConfigurationValue &value) {
			os << key << '=';
			switch (value.getType()) {
				case ConfigurationValueType::Integer:
					os << value.getInt();
					break;
				case ConfigurationValueType::Real:
					os << value.getReal();
					break;
				case ConfigurationValueType::Boolean:
					os << (value.getBool() ? "true" : "false");
					break;
				case ConfigurationValueType::String:
					os << '\"' << value.getString() << '\"';
					break;
				case ConfigurationValueType::None:
					break;
			}
			os << std::endl;
    });
  }

	size_t skipWhitespaces(const char *line, size_t start) {
		while (start < strlen(line) && isspace(line[start])) {
			start++;
		}
		return start;
	}

	std::unique_ptr<ConfigManager> ConfigManagerIO::load(std::istream &is,
	                                                   std::ostream &err,
	                                                   ConfigManager *man) {
		std::unique_ptr<ConfigManager> new_man = nullptr;
		if (man == nullptr) {
			new_man = std::make_unique<ConfigManager>();
			man = new_man.get();
		}
		ConfiguationFlatDictionary *entry = nullptr;
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
					if (!value.is(ConfigurationValueType::None)) {
						entry->put(key, std::move(value));
					}
				}
			}
		}
		return new_man;
	}

	ConfigurationValue ConfigManagerIO::parseValue(const char *input) {
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
					// +/- on the first place is allowed for both integers and reals
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
}