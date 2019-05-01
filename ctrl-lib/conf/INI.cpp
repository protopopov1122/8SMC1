/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

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

#include "calx/ctrl-lib/conf/INI.h"
#include "calx/ctrl-lib/conf/ConfigManager.h"
#include <iostream>
#include <cstring>
#include <regex>

namespace CalX {

	void INIConfiguration::store(ConfigurationCatalogue &config,
	                             std::ostream &os) {
		config.visit(
		    [&](const std::string &entryName, ConfiguationFlatDictionary &entry) {
			    os << '[' << entryName << ']' << std::endl;
			    INIConfiguration::store(entry, os);
			    os << std::endl;
		    });
	}

	void INIConfiguration::store(ConfiguationFlatDictionary &entry,
	                             std::ostream &os) {
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

	static void leftTrimWhitespaces(std::string &str) {
		static std::regex WHITESPACE(R"(^\s+)");
		std::smatch match;
		if (std::regex_search(str, match, WHITESPACE) && !match.empty()) {
			str.erase(0, match.length());
		}
	}

	static void rightTrimWhitespaces(std::string &str) {
		while (!str.empty() && iswspace(str.at(str.length() - 1))) {
			str.erase(str.length() - 1, str.length());
		}
	}

	static bool regexMatches(const std::string &str, const std::regex &rx) {
		std::smatch match;
		return std::regex_search(str, match, rx) && !match.empty();
	}

	static std::regex LITERAL(R"(^[a-zA-Z_]+\w*)");

	ConfiguationFlatDictionary *parseSectionDeclaration(
	    std::string &line, std::ostream &err, ConfigurationCatalogue &manager,
	    uint32_t lineNumber) {
		line.erase(0, 1);
		leftTrimWhitespaces(line);
		std::smatch match;
		std::regex_search(line, match, LITERAL);
		if (match.empty()) {
			err << "Expected section identifier at line " << lineNumber << std::endl;
			return nullptr;
		} else {
			std::string sectionName = match.str();
			line.erase(0, sectionName.length());
			leftTrimWhitespaces(line);
			if (line.at(0) == ']') {
				return manager.getEntry(sectionName);
			} else {
				err << "Expected \']\' at the end of line " << lineNumber << std::endl;
				return nullptr;
			}
		}
	}
	
	static std::string &ltrim_string(std::string &str, const std::string& chars = "\t\n\v\f\r ") {
		str.erase(0, str.find_first_not_of(chars));
		return str;
	}
	 
	static std::string &rtrim_string(std::string &str, const std::string& chars = "\t\n\v\f\r ") {
		str.erase(str.find_last_not_of(chars) + 1);
		return str;
	}
	 
	static std::string &trim_string(std::string &str, const std::string& chars = "\t\n\v\f\r ") {
		return ltrim_string(rtrim_string(str, chars), chars);
	}

	ConfigurationValue parseValue(std::string &line) {
		static std::regex INTEGER(R"(^-?[0-9]+)");
		static std::regex FLOAT(R"(^-?[0-9]+\.[0-9]+)");
		static std::regex BOOL(R"(^true|false)");
		static std::regex STRING(R"(^(["'])(?:(?=(\\?))\2.)*?\1)");

		std::smatch match;
		if (std::regex_search(line, match, FLOAT) && !match.empty()) {
			return ConfigurationValue(std::stod(match.str()));
		} else if (std::regex_search(line, match, INTEGER) && !match.empty()) {
			return ConfigurationValue(
			    static_cast<int_conf_t>(std::stoll(match.str())));
		} else if (std::regex_search(line, match, BOOL) && !match.empty()) {
			return ConfigurationValue(match.str().compare("true") == 0);
		} else if (std::regex_search(line, match, STRING) && !match.empty()) {
			std::string str = match.str().substr(1, match.length() - 2);
			return ConfigurationValue(str);
		} else {
			std::size_t idx = line.find('#');
			std::size_t value_length = idx == std::string::npos ? line.length() : idx;
			std::string str = line.substr(0, value_length);
			return ConfigurationValue(trim_string(str));
		}
	}

	static void parseSectionEntry(std::string &line, std::ostream &err,
	                              ConfiguationFlatDictionary &entry,
	                              uint32_t lineNumber) {
		std::smatch keyMatch;
		std::regex_search(line, keyMatch, LITERAL);
		std::string key = keyMatch.str();
		line.erase(0, key.length());
		leftTrimWhitespaces(line);
		if (line.at(0) != '=') {
			err << "Expected \'=\' at line " << lineNumber << std::endl;
			return;
		}
		line.erase(0, 1);
		leftTrimWhitespaces(line);
		ConfigurationValue value = parseValue(line);
		entry.put(key, value);
	}

	std::unique_ptr<ConfigurationCatalogue> INIConfiguration::load(
	    std::istream &is, std::ostream &err, ConfigurationCatalogue *manager) {
		std::unique_ptr<ConfigurationCatalogue> newManager = nullptr;
		if (manager == nullptr) {
			newManager = std::make_unique<ConfigManager>();
			manager = newManager.get();
		}

		ConfiguationFlatDictionary *entry = nullptr;
		for (uint32_t lineNumber = 1; is.good(); lineNumber++) {
			std::string line;
			std::getline(is, line);
			leftTrimWhitespaces(line);
			rightTrimWhitespaces(line);
			if (!line.empty() && line.at(0) == '[') {
				entry = parseSectionDeclaration(line, err, *manager, lineNumber);
			} else if (regexMatches(line, LITERAL)) {
				if (entry != nullptr) {
					parseSectionEntry(line, err, *entry, lineNumber);
				} else {
					err << "Entry not defined at line " << lineNumber << std::endl;
				}
			} else if (!line.empty() && line.at(0) != '#') {
				err << "Unknown statement at line " << lineNumber << std::endl;
			}
		}
		return newManager;
	}
}  // namespace CalX