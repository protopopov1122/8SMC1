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

#ifndef CALX_CTRL_LIB_CONF_SETTINGS_H_
#define CALX_CTRL_LIB_CONF_SETTINGS_H_

#include "ctrl-lib/conf/Dictionary.h"
#include <map>

namespace CalX {

  class SettingsRepository {
   public:
    virtual ~SettingsRepository() = default;
    virtual ConfigurationCatalogue &getSettings() = 0;
  };

  class SettingsConfigurationEntry : public ConfiguationFlatDictionary {
   public:
    SettingsConfigurationEntry(ConfigurationCatalogue *, ConfigurationCatalogue &, const std::string &, bool);
    const ConfigurationValue &get(const std::string &) const override;
    bool has(const std::string &) const override;
    bool put(const std::string &, const ConfigurationValue &) override;
    bool remove(const std::string &) override;
		void visit(std::function<void (const std::string &, const ConfigurationValue &)>) const override;
   private:
    ConfiguationFlatDictionary *master;
    ConfiguationFlatDictionary *fallback;
  };

  class SettingsConfiguration : public ConfigurationCatalogue {
   public:
    SettingsConfiguration(SettingsRepository *, ConfigurationCatalogue &);
    SettingsConfiguration(ConfigurationCatalogue *, ConfigurationCatalogue &);
    ConfiguationFlatDictionary *getEntry(const std::string &, bool = true) override;
    bool hasEntry(const std::string &) const override;
    bool removeEntry(const std::string &) override;
		void visit(std::function<void (const std::string &, ConfiguationFlatDictionary &)>) const override;
   private:
    ConfigurationCatalogue *master;
    ConfigurationCatalogue &fallback;
    std::map<std::string, std::unique_ptr<SettingsConfigurationEntry>> entries;
  };
}

#endif