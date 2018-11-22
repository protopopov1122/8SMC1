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

#ifndef CALX_CTRL_LIB_CONF_DICTIONARY_H_
#define CALX_CTRL_LIB_CONF_DICTIONARY_H_

#include "ctrl-lib/conf/Value.h"
#include "ctrl-lib/conf/Event.h"
#include <memory>

namespace CalX {

  class ConfiguationFlatDictionary : public EventSource<std::shared_ptr<FlatDictionaryListener>> {
   public:
    virtual ~ConfiguationFlatDictionary() = default;
    virtual const ConfigurationValue &get(const std::string &) const = 0;
    virtual bool has(const std::string &) const = 0;
    virtual bool put(const std::string &, const ConfigurationValue &) = 0;
    virtual bool remove(const std::string &) = 0;
		virtual void visit(std::function<void (const std::string &, const ConfigurationValue &)>) const = 0;

		bool is(const std::string &, ConfigurationValueType) const;
		int_conf_t getInt(const std::string &, int_conf_t = 0) const;
		real_conf_t getReal(const std::string &, real_conf_t = 0) const;
		bool getBool(const std::string &, bool = false) const;
		std::string getString(const std::string &, const std::string & = "") const;
  };

  class ConfigurationCatalogue : public EventSource<std::shared_ptr<CatalogueListener>> {
   public:
    virtual ~ConfigurationCatalogue() = default;
    virtual ConfiguationFlatDictionary *getEntry(const std::string &, bool = true) = 0;
    virtual bool hasEntry(const std::string &) const = 0;
    virtual bool removeEntry(const std::string &) = 0;
		virtual void visit(std::function<void (const std::string &, ConfiguationFlatDictionary &)>) const = 0;
  };
}

#endif