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

#ifndef _CALX_CTRL_LIB_CONFIG_MANAGER_H_
#define _CALX_CTRL_LIB_CONFIG_MANAGER_H_

#include "ctrl-lib/CtrlCore.h"
#include "ctrl-lib/EventListener.h"
#include <cinttypes>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <memory>

/* This file contains configuration manager - configuration parameter storage.
   It represents structure similar to INI file, but all values have types.
   I didn't used external libraries because I want to keep core code
   dependency free. */

namespace CalX {

	class ConfigValidator;  // Forward referencing

	typedef int64_t int_conf_t;
	typedef double real_conf_t;

	enum class ConfigValueType { Integer, Real, String, Boolean };

	class ConfigValue {
	 public:
		ConfigValue(ConfigValueType tp) : type(tp) {}
		virtual ~ConfigValue() = default;
		ConfigValueType getType() const {
			return this->type;
		}
		virtual std::string toString() const = 0;

	 protected:
		ConfigValueType type;
	};

	class IntegerConfigValue : public ConfigValue {
	 public:
		IntegerConfigValue(int_conf_t v)
		    : ConfigValue::ConfigValue(ConfigValueType::Integer), value(v) {}

		int_conf_t getValue() const {
			return this->value;
		}
		virtual std::string toString() const {
			return std::to_string(this->value);
		}

	 private:
		int_conf_t value;
	};

	class RealConfigValue : public ConfigValue {
	 public:
		RealConfigValue(real_conf_t v)
		    : ConfigValue::ConfigValue(ConfigValueType::Real), value(v) {}

		real_conf_t getValue() const {
			return this->value;
		}
		virtual std::string toString() const {
			return std::to_string(this->value);
		}

	 private:
		real_conf_t value;
	};

	class BoolConfigValue : public ConfigValue {
	 public:
		BoolConfigValue(bool v)
		    : ConfigValue::ConfigValue(ConfigValueType::Boolean), value(v) {}

		bool getValue() const {
			return this->value;
		}
		virtual std::string toString() const {
			return std::string(this->value ? "true" : "false");
		}

	 private:
		bool value;
	};

	class StringConfigValue : public ConfigValue {
	 public:
		StringConfigValue(std::string v)
		    : ConfigValue::ConfigValue(ConfigValueType::String), value(v) {}

		std::string getValue() const {
			return this->value;
		}
		virtual std::string toString() const {
			return this->value;
		}

	 private:
		std::string value;
	};

	class ConfigEntry {
	 public:
		ConfigEntry(ConfigManager *, std::string);
		std::string getEntryName() const;

		std::shared_ptr<ConfigValue> get(std::string) const;
		bool has(std::string) const;
		bool put(std::string, std::shared_ptr<ConfigValue>);
		bool remove(std::string);
		bool is(std::string, ConfigValueType) const;
		void store(std::ostream &) const;

		int_conf_t getInt(std::string, int_conf_t = 0) const;
		real_conf_t getReal(std::string, real_conf_t = 0) const;
		bool getBool(std::string, bool = false) const;
		std::string getString(std::string, std::string = "") const;

		void getContent(
		    std::vector<std::pair<std::string, std::shared_ptr<ConfigValue>>> &)
		    const;

	 private:
		ConfigManager *config;
		std::string name;
		std::map<std::string, std::shared_ptr<ConfigValue>> content;
	};

	class ConfigManager {
	 public:
		ConfigManager();
		virtual ~ConfigManager() = default;

		std::shared_ptr<ConfigEntry> getEntry(std::string, bool = true);
		bool hasEntry(std::string) const;
		bool removeEntry(std::string);
		void store(std::ostream &) const;
		void getEntries(std::vector<std::shared_ptr<ConfigEntry>> &) const;
		void setValidator(std::shared_ptr<ConfigValidator>);
		std::shared_ptr<ConfigValidator> getValidator() const;
		bool validate(std::shared_ptr<ConfigValidator> = nullptr);

		void addEventListener(std::shared_ptr<ConfigEventListener>);
		void removeEventListener(std::shared_ptr<ConfigEventListener>);
		std::vector<std::shared_ptr<ConfigEventListener>> &getEventListeners();

		static std::unique_ptr<ConfigManager> load(std::istream &, std::ostream &,
		                                           ConfigManager * = nullptr);
		static std::unique_ptr<ConfigValue> parseValue(const char *);

	 private:
		std::map<std::string, std::shared_ptr<ConfigEntry>> entries;
		std::shared_ptr<ConfigValidator> validator;
		std::vector<std::shared_ptr<ConfigEventListener>> listeners;
	};
}  // namespace CalX

#endif
