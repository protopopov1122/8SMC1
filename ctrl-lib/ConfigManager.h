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


#ifndef _CALX_CTRL_LIB_CONFIG_MANAGER_H_
#define _CALX_CTRL_LIB_CONFIG_MANAGER_H_

#include <string>
#include <map>
#include <cinttypes>
#include <iostream>
#include "CtrlCore.h"

/* This file contains configuration manager - configuration parameter storage.
   It represents structure similar to INI file, but all values have types.
   I didn't used external libraries because I want to keep core code
   dependency free. */

namespace CalX {

	typedef int64_t int_conf_t;
	typedef double real_conf_t;

	enum class ConfigValueType {
		Integer, Real, String, Boolean
	};

	class ConfigValue {
		public:
			ConfigValue(ConfigValueType type) {this->type = type;}
			virtual ~ConfigValue() {}
			ConfigValueType getType() {return this->type;}
		protected:
			ConfigValueType type;
	};

	class IntegerConfigValue : public ConfigValue {
		public:
			IntegerConfigValue(int_conf_t v)
				: ConfigValue::ConfigValue(ConfigValueType::Integer) {
				this->value = v;
			}

			int_conf_t getValue() {return this->value;}
		private:
			int_conf_t value;
	};
	
	class RealConfigValue : public ConfigValue {
		public:
			RealConfigValue(real_conf_t v)
				: ConfigValue::ConfigValue(ConfigValueType::Real) {
				this->value = v;
			}

			real_conf_t getValue() {return this->value;}
		private:
			real_conf_t value;
	};

	class BoolConfigValue : public ConfigValue {
		public:
			BoolConfigValue(bool v)
				: ConfigValue::ConfigValue(ConfigValueType::Boolean) {
				this->value = v;
			}

			bool getValue() {return this->value;}
		private:
			bool value;
	};

	class StringConfigValue : public ConfigValue {
		public:
			StringConfigValue(std::string v)
				: ConfigValue::ConfigValue(ConfigValueType::String) {
				this->value = v;
			}

			std::string getValue() {return this->value;}
		private:
			std::string value;
	};
	

	class ConfigEntry {
		public:
			ConfigEntry(std::string);
			virtual ~ConfigEntry();
			std::string getEntryName();
		
			ConfigValue *get(std::string);
			bool has(std::string);
			bool put(std::string, ConfigValue*);
			bool remove(std::string);
			bool is(std::string, ConfigValueType);
			void store(std::ostream*);
			
			int_conf_t getInt(std::string, int_conf_t = 0);
			real_conf_t getReal(std::string, real_conf_t = 0);
			bool getBool(std::string, bool = false);
			std::string getString(std::string, std::string = "");
		private:
			std::string name;
			std::map<std::string, ConfigValue*> content;
	};

	class ConfigManager {
		public:
			ConfigManager();
			virtual ~ConfigManager();
		
			ConfigEntry *getEntry(std::string, bool = true);
			bool hasEntry(std::string);
			void store(std::ostream*);
			
			static ConfigManager *load(std::istream*, std::ostream*);
		private:
			std::map<std::string, ConfigEntry*> entries;
	};

}
	
#endif