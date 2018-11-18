#include <functional>
#include <fstream>
#include <iostream>
#include "ctrl-lib/conf/Settings.h"

namespace CalX {

  class SettingsConfigListener : public ConfigEventListener {
   public:
    SettingsConfigListener(std::function<void()> exportFn)
      : exportFn(exportFn) {}
    
		void entryAdded(ConfigManager *conf, std::string entry) override {
      this->exportFn();
    }

		void entryRemoved(ConfigManager *conf, std::string entry) override {
      this->exportFn();
    }

		void keyAdded(ConfigManager *conf, std::string entry, std::string key) override {
      this->exportFn();
    }

		void keyRemoved(ConfigManager *conf, std::string entry, std::string key) override {
      this->exportFn();
    }

		void keyChanged(ConfigManager *conf, std::string entry, std::string key) override {
      this->exportFn();
    }
   private:
    std::function<void()> exportFn;
  };

  SettingsFileRepository::SettingsFileRepository(const std::string &path)
    : repo_path(path) {
    this->importRepo();
  }

  const std::string &SettingsFileRepository::getRepositoryPath() const {
    return this->repo_path;
  }

  ConfigManager &SettingsFileRepository::getSettings() {
    return *this->settings;
  }

  void SettingsFileRepository::importRepo() {
    std::ifstream is(this->repo_path);
    if (is.good()) {
      this->settings = ConfigManagerIO::load(is, std::cout);
    } else {
      this->settings = std::make_unique<ConfigManager>();
    }
    this->settings->addEventListener(std::make_shared<SettingsConfigListener>([this]() { this->exportRepo(); }));
  }

  void SettingsFileRepository::exportRepo() {
    std::ofstream os(this->repo_path);
    ConfigManagerIO::store(*this->settings, os);
  }
}