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
#include <iosfwd>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <variant>

/* This file contains configuration manager - configuration parameter storage.
   It represents structure similar to INI file, but all values have types.
   I didn't used external libraries because I want to keep core code
   dependency free. */

namespace CalX {

	class ConfigValidator;  // Forward referencing

	typedef int64_t int_conf_t;
	typedef double real_conf_t;

	enum class ConfigValueType { Integer, Real, String, Boolean, None };

	class ConfigurationValue {
	 public:
	 	ConfigurationValue();
		ConfigurationValue(int_conf_t);
		ConfigurationValue(real_conf_t);
		ConfigurationValue(bool);
		ConfigurationValue(std::string);

		ConfigValueType getType() const;
		bool is(ConfigValueType) const;

		int_conf_t getInt(int_conf_t = 0) const;
		real_conf_t getReal(real_conf_t = 0.0) const;
		bool getBool(bool = false) const;
		std::string getString(std::string = "") const;
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
		operator std::string () const {
			return this->toString();
		}

		static ConfigurationValue Empty;
	 private:
		ConfigValueType type;
		std::variant<int_conf_t, real_conf_t, bool, std::string> value;
	};

	class ConfigEntry {
	 public:
		ConfigEntry(ConfigManager &, std::string);
		const std::string &getEntryName() const;

		const ConfigurationValue &get(std::string) const;
		bool has(std::string) const;
		bool put(std::string, ConfigurationValue);
		bool remove(std::string);
		bool is(std::string, ConfigValueType) const;
		void store(std::ostream &) const;

		int_conf_t getInt(std::string, int_conf_t = 0) const;
		real_conf_t getReal(std::string, real_conf_t = 0) const;
		bool getBool(std::string, bool = false) const;
		std::string getString(std::string, std::string = "") const;

		void getContent(
		    std::vector<std::pair<std::string, ConfigurationValue>> &)
		    const;

	 private:
		ConfigManager &config;
		std::string name;
		std::map<std::string, ConfigurationValue> content;
	};

	class ConfigManager : public EventSource<std::shared_ptr<ConfigEventListener>> {
	 public:
		ConfigManager();

		ConfigEntry *getEntry(std::string, bool = true);
		bool hasEntry(std::string) const;
		bool removeEntry(std::string);
		void store(std::ostream &) const;
		void getEntries(std::vector<ConfigEntry *> &) const;
		void setValidator(std::shared_ptr<ConfigValidator>);
		std::shared_ptr<ConfigValidator> getValidator() const;
		bool validate(ConfigValidator * = nullptr);

		std::vector<std::shared_ptr<ConfigEventListener>> &getEventListeners();

		static std::unique_ptr<ConfigManager> load(std::istream &, std::ostream &,
		                                           ConfigManager * = nullptr);

	 private:
		static ConfigurationValue parseValue(const char *);

		std::map<std::string, std::unique_ptr<ConfigEntry>> entries;
		std::shared_ptr<ConfigValidator> validator;
	};
}  // namespace CalX

#endif
