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

#ifndef CALX_CTRL_LIB_CONFIG_VALIDATOR_H_
#define CALX_CTRL_LIB_CONFIG_VALIDATOR_H_

#include "calx/ctrl-lib/CtrlCore.h"
#include "calx/ctrl-lib/conf/ConfigManager.h"
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
		ConfigValueValidator(ConfigurationValueType, bool = false);
		ConfigurationValueType getType();
		virtual bool validate(const ConfigurationValue &);

	 private:
		ConfigurationValueType type;
	};

	class ConfigKeyValidator : public AbstractValidator {
	 public:
		ConfigKeyValidator(const std::string &,
		                   std::unique_ptr<ConfigValueValidator>, bool = false);
		std::string getKey();
		ConfigValueValidator *getValue();
		bool validate(ConfiguationFlatDictionary *);

	 private:
		std::string key;
		std::unique_ptr<ConfigValueValidator> value;
	};

	class ConfigEntryValidator : public AbstractValidator {
	 public:
		ConfigEntryValidator(const std::string &, bool = false);
		std::string getEntryName();
		bool validate(ConfigurationCatalogue *);
		void addKeyValidator(std::unique_ptr<ConfigKeyValidator>);

	 private:
		std::string name;
		std::vector<std::unique_ptr<ConfigKeyValidator>> keys;
	};

	class ConfigValidator : public AbstractValidator {
	 public:
		ConfigValidator(bool = true);
		void addEntryValidator(std::unique_ptr<ConfigEntryValidator>);
		bool validate(ConfigurationCatalogue *);

	 private:
		std::vector<std::unique_ptr<ConfigEntryValidator>> entries;
	};
}  // namespace CalX

#endif
