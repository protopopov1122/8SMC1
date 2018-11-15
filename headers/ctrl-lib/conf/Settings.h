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