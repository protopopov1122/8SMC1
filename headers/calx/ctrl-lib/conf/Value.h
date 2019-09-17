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

#ifndef CALX_CTRL_LIB_CONF_VALUE_H_
#define CALX_CTRL_LIB_CONF_VALUE_H_

#include "calx/ctrl-lib/CtrlCore.h"
#include <variant>
#include <functional>

namespace CalX {

	typedef int64_t int_conf_t;
	typedef double real_conf_t;

	enum class ConfigurationValueType { Integer, Real, String, Boolean, None };

	class ConfiguationFlatDictionary;  // Forward reference

	class ConfigurationValue {
	 public:
		ConfigurationValue();
		ConfigurationValue(const ConfigurationValue &);
		ConfigurationValue(int_conf_t);
		ConfigurationValue(real_conf_t);
		ConfigurationValue(bool);
		ConfigurationValue(const std::string &);

		ConfigurationValueType getType() const;
		bool is(ConfigurationValueType) const;

		int_conf_t getInt(int_conf_t = 0) const;
		real_conf_t getReal(real_conf_t = 0.0) const;
		bool getBool(bool = false) const;
		const std::string &getString(const std::string & = "") const;
		std::string toString() const;

		operator int_conf_t() const {
			return this->getInt();
		}

		operator real_conf_t() const {
			return this->getReal();
		}

		operator bool() const {
			return this->getBool();
		}

		operator std::string() const {
			return this->toString();
		}

		static ConfigurationValue Empty;

	 private:
		ConfigurationValueType type;
		std::variant<int_conf_t, real_conf_t, bool, std::string> value;
	};
}  // namespace CalX

#endif