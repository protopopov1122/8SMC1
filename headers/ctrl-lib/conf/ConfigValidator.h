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


#ifndef CALX_CTRL_LIB_CONFIG_VALIDATOR_H_
#define CALX_CTRL_LIB_CONFIG_VALIDATOR_H_

#include <vector>
#include <string>
#include "ctrl-lib/CtrlCore.h"
#include "ctrl-lib/conf/ConfigManager.h"

namespace CalX {

	class AbstractValidator {
		public:
			AbstractValidator(bool);
			virtual ~AbstractValidator();
			bool isOptional();
		protected:
			bool optional;
	};

	class ConfigValueValidator : public AbstractValidator {
		public:
			ConfigValueValidator(ConfigValueType, bool = false);
			virtual ~ConfigValueValidator();
			ConfigValueType getType();
			virtual bool validate(ConfigValue*);
		private:
			ConfigValueType type;
	};

	class ConfigKeyValidator : public AbstractValidator {
		public:
			ConfigKeyValidator(std::string, ConfigValueValidator*, bool = false);
			virtual ~ConfigKeyValidator();
			std::string getKey();
			ConfigValueValidator *getValue();
			bool validate(ConfigEntry*);
		private:
			std::string key;
			ConfigValueValidator *value;
	};

	class ConfigEntryValidator : public AbstractValidator {
		public:
			ConfigEntryValidator(std::string, bool = false);
			virtual ~ConfigEntryValidator();
			std::string getEntryName();
			bool validate(ConfigManager*);
			void addKeyValidator(ConfigKeyValidator*);
		private:
			std::string name;
			std::vector<ConfigKeyValidator*> keys;
	};

	class ConfigValidator : public AbstractValidator {
		public:
			ConfigValidator(bool = true);
			virtual ~ConfigValidator();
			void addEntryValidator(ConfigEntryValidator*);
			bool validate(ConfigManager*);
		private:
			std::vector<ConfigEntryValidator*> entries;
	};
}

#endif
