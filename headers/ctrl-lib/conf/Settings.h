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

#include "ctrl-lib/conf/ConfigManager.h"

namespace CalX {

  class SettingsRepository {
   public:
    virtual ~SettingsRepository() = default;
    virtual ConfigManager &getSettings() = 0;
  };

  class SettingsFileRepository : public SettingsRepository {
   public:
    SettingsFileRepository(const std::string &);

    const std::string &getRepositoryPath() const;
    ConfigManager &getSettings() override;
   private:
    void importRepo();
    void exportRepo();

    std::string repo_path;
    std::unique_ptr<ConfigManager> settings;
  };
}

#endif