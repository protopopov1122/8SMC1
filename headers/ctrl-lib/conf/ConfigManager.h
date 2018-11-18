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
#include "ctrl-lib/conf/Value.h"
#include "ctrl-lib/conf/Dictionary.h"
#include <cinttypes>
#include <iosfwd>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <functional>

/* This file contains configuration manager - configuration parameter storage.
   It represents structure similar to INI file, but all values have types.
   I didn't used external libraries because I want to keep core code
   dependency free. */

namespace CalX {

	class ConfigEntry : public ConfiguationFlatDictionary {
	 public:
		ConfigEntry(ConfigManager &, const std::string &);

		const ConfigurationValue &get(const std::string &) const override;
		bool has(const std::string &) const override;
		bool put(const std::string &, const ConfigurationValue &) override;
		bool remove(const std::string &) override;

		void visit(std::function<void (const std::string &, const ConfigurationValue &)>) const override;

	 private:
		ConfigManager &config;
		std::string entryName;
		std::map<std::string, ConfigurationValue> content;
	};

	class ConfigManager : public ConfigurationCatalogue, public EventSource<std::shared_ptr<ConfigEventListener>> {
	 public:
		ConfigManager();

		ConfiguationFlatDictionary *getEntry(const std::string &, bool = true) override;
		bool hasEntry(const std::string &) const override;
		bool removeEntry(const std::string &) override;
		void visit(std::function<void (const std::string &, ConfiguationFlatDictionary &)>) const override;

		std::vector<std::shared_ptr<ConfigEventListener>> &getEventListeners();
	 private:
		std::map<std::string, std::unique_ptr<ConfigEntry>> entries;
	};

	class ConfigManagerIO {
	 public:
		static void store(ConfigManager &, std::ostream &);
		static std::unique_ptr<ConfigManager> load(std::istream &, std::ostream &,
		                                           ConfigManager * = nullptr);
	 private:
		static void store(ConfiguationFlatDictionary &, std::ostream &);
		static ConfigurationValue parseValue(const char *);
	};
}  // namespace CalX

#endif
