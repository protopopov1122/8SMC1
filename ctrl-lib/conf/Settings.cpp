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

#include <functional>
#include <fstream>
#include <iostream>
#include "ctrl-lib/conf/Settings.h"

namespace CalX {

  class SettingsConfigListener : public CatalogueListener {
   public:
    SettingsConfigListener(std::function<void()> exportFn)
      : exportFn(exportFn) {}
    
		void entryAdd(ConfigurationCatalogue *conf, const std::string &entry) override {
      this->exportFn();
    }

		void entryRemove(ConfigurationCatalogue *conf, const std::string &entry) override {
      this->exportFn();
    }

		void keyAdd(ConfigurationCatalogue *conf, const std::string &entry, const std::string &key) override {
      this->exportFn();
    }

		void keyRemove(ConfigurationCatalogue *conf, const std::string &entry, const std::string &key) override {
      this->exportFn();
    }

		void keyChange(ConfigurationCatalogue *conf, const std::string &entry, const std::string &key) override {
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