/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CALX_CTRL_LIB_CONF_FILE_SETTINGS_H_
#define CALX_CTRL_LIB_CONF_FILE_SETTINGS_H_

#include "calx/ctrl-lib/conf/Settings.h"
#include "calx/ctrl-lib/conf/ConfigManager.h"
#include <iostream>
#include <functional>
#include <fstream>

namespace CalX {

	class SettingsConfigListener : public CatalogueListener {
	 public:
		SettingsConfigListener(std::function<void()> exportFn)
		    : exportFn(exportFn) {}

		void entryAdd(ConfigurationCatalogue *conf,
		              const std::string &entry) override {
			this->exportFn();
		}

		void entryRemove(ConfigurationCatalogue *conf,
		                 const std::string &entry) override {
			this->exportFn();
		}

		void keyAdd(ConfigurationCatalogue *conf, const std::string &entry,
		            const std::string &key) override {
			this->exportFn();
		}

		void keyRemove(ConfigurationCatalogue *conf, const std::string &entry,
		               const std::string &key) override {
			this->exportFn();
		}

		void keyChange(ConfigurationCatalogue *conf, const std::string &entry,
		               const std::string &key) override {
			this->exportFn();
		}

	 private:
		std::function<void()> exportFn;
	};

	template<typename IO>
	class SettingsFileRepository : public SettingsRepository {
	 public:
		SettingsFileRepository(const std::string &path) : repo_path(path) {
			this->importRepo();
		}

		const std::string &getRepositoryPath() const {
			return this->repo_path;
		}

		ConfigurationCatalogue &getSettings() {
			return *this->settings;
		}

	 private:
		void importRepo() {
			std::ifstream is(this->repo_path);
			if (is.good()) {
				this->settings = IO::load(is, std::cout);
			} else {
				this->settings = std::make_unique<ConfigManager>();
			}
			this->settings->addEventListener(std::make_shared<SettingsConfigListener>(
			    [this]() { this->exportRepo(); }));
		}

		void exportRepo() {
			std::ofstream os(this->repo_path);
			IO::store(*this->settings, os);
		}

		std::string repo_path;
		std::unique_ptr<ConfigurationCatalogue> settings;
	};
}  // namespace CalX

#endif