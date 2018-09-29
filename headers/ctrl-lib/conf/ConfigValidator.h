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

#ifndef CALX_CTRL_LIB_CONFIG_VALIDATOR_H_
#define CALX_CTRL_LIB_CONFIG_VALIDATOR_H_

#include "ctrl-lib/CtrlCore.h"
#include "ctrl-lib/conf/ConfigManager.h"
#include <string>
#include <vector>

/* Config validator allows configuration validation. It is
   not used by the project. */

namespace CalX {

	class AbstractValidator {
	 public:
		AbstractValidator(bool);
		virtual ~AbstractValidator() = default;
		bool isOptional();

	 protected:
		bool optional;
	};

	class ConfigValueValidator : public AbstractValidator {
	 public:
		ConfigValueValidator(ConfigValueType, bool = false);
		ConfigValueType getType();
		virtual bool validate(std::shared_ptr<ConfigValue>);

	 private:
		ConfigValueType type;
	};

	class ConfigKeyValidator : public AbstractValidator {
	 public:
		ConfigKeyValidator(std::string, std::unique_ptr<ConfigValueValidator>,
		                   bool = false);
		std::string getKey();
		ConfigValueValidator *getValue();
		bool validate(std::shared_ptr<ConfigEntry>);

	 private:
		std::string key;
		std::unique_ptr<ConfigValueValidator> value;
	};

	class ConfigEntryValidator : public AbstractValidator {
	 public:
		ConfigEntryValidator(std::string, bool = false);
		std::string getEntryName();
		bool validate(ConfigManager *);
		void addKeyValidator(std::unique_ptr<ConfigKeyValidator>);

	 private:
		std::string name;
		std::vector<std::unique_ptr<ConfigKeyValidator>> keys;
	};

	class ConfigValidator : public AbstractValidator {
	 public:
		ConfigValidator(bool = true);
		void addEntryValidator(std::unique_ptr<ConfigEntryValidator>);
		bool validate(ConfigManager *);

	 private:
		std::vector<std::unique_ptr<ConfigEntryValidator>> entries;
	};
}  // namespace CalX

#endif
