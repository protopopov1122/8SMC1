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

#ifndef CALX_CTRL_LIB_CONF_EVENT_H_
#define CALX_CTRL_LIB_CONF_EVENT_H_

#include "ctrl-lib/Event.h"

namespace CalX {

  class ConfiguationFlatDictionary; // Forward referencing
  class ConfigurationCatalogue; // Forward referencing

  class FlatDictionaryListener {
   public:
    virtual ~FlatDictionaryListener() = default;
    virtual void keyAdd(ConfiguationFlatDictionary &dict, const std::string &key) {};
    virtual void keyChange(ConfiguationFlatDictionary &dict, const std::string &key) {};
    virtual void keyRemove(ConfiguationFlatDictionary &dict, const std::string &key) {};
  };

  class CatalogueListener {
   public:
    virtual ~CatalogueListener() = default;
    virtual void entryAdd(ConfigurationCatalogue *cat, const std::string &entry) {};
    virtual void entryRemove(ConfigurationCatalogue *cat, const std::string &entry) {};
    virtual void keyAdd(ConfigurationCatalogue *cat, const std::string &entry, const std::string &key) {};
    virtual void keyChange(ConfigurationCatalogue *cat, const std::string &entry, const std::string &key) {};
    virtual void keyRemove(ConfigurationCatalogue *cat, const std::string &entry, const std::string &key) {};
  };
}

#endif