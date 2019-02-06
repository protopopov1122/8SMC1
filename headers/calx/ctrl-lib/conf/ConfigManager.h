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

#ifndef _CALX_CTRL_LIB_CONFIG_MANAGER_H_
#define _CALX_CTRL_LIB_CONFIG_MANAGER_H_

#include "calx/ctrl-lib/CtrlCore.h"
#include "calx/ctrl-lib/EventListener.h"
#include "calx/ctrl-lib/conf/Value.h"
#include "calx/ctrl-lib/conf/Dictionary.h"
#include "calx/ctrl-lib/conf/INI.h"
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
		ConfigEntry();

		const ConfigurationValue &get(const std::string &) const override;
		bool has(const std::string &) const override;
		bool put(const std::string &, const ConfigurationValue &) override;
		bool remove(const std::string &) override;

		void visit(std::function<void(const std::string &,
		                              const ConfigurationValue &)>) const override;

	 private:
		std::map<std::string, ConfigurationValue> content;
	};

	class ConfigManager : public ConfigurationCatalogue {
	 public:
		ConfigManager();

		ConfiguationFlatDictionary *getEntry(const std::string &,
		                                     bool = true) override;
		bool hasEntry(const std::string &) const override;
		bool removeEntry(const std::string &) override;
		void visit(
		    std::function<void(const std::string &, ConfiguationFlatDictionary &)>)
		    const override;

	 private:
		std::map<std::string, std::unique_ptr<ConfigEntry>> entries;
	};
}  // namespace CalX

#endif
