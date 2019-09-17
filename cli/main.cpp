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

#include "calx/cli/DevCLI.h"
#include "calx/ctrl-lib/SignalHandler.h"
#include "calx/ctrl-lib/conf/ConfigManager.h"
#include "calx/ctrl-lib/SystemManager.h"
#include "calx/ctrl-lib/device/DeviceManager.h"
#include "calx/ctrl-lib/translator/CoordTranslator.h"
#include <cinttypes>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

/* Creates device manager and runs command line*/

using namespace CalX;

class EchoCMD : public CLICommand {
 public:
	void execute(CLI *cli, std::vector<std::string> &args) override {
		for (std::size_t i = 0; i < args.size(); i++) {
			std::cout << args.at(i) << " ";
		}
		std::cout << std::endl;
	}
};

class HelpCMD : public CLICommand {
 public:
	void execute(CLI *cli, std::vector<std::string> &args) override {
		std::cout << "See cli/README.md in project repo to get CLI manual"
		          << std::endl;
	}
};

int main(int argc, char **argv) {
	std::unique_ptr<DeviceManager> devman =
	    std::unique_ptr<DeviceManager>(getDeviceManager());
	std::unique_ptr<ConfigurationCatalogue> conf = nullptr;
	std::ifstream cnf("main.conf.ini");
	if (!cnf.good()) {
		std::cout << "Can't load configuration, using default values." << std::endl;
		conf = std::make_unique<ConfigManager>();
	} else {
		conf = INIConfiguration::load(cnf, std::cout);
	}
	cnf.close();
	DefaultSystemManager sysman(std::move(devman), std::move(conf));
	VectorTaskSet taskSet;
	setup_signals(sysman);
	CLI cli(std::cout, std::cin);
	cli.addCommand("echo", std::make_unique<EchoCMD>());
	cli.addCommand("ls", std::make_unique<LsCommand>(sysman, taskSet));
	cli.addCommand("x", std::make_unique<HaltCommand>(sysman, taskSet));
	cli.addCommand("dev", std::make_unique<MotorCommand>(sysman, taskSet));
	cli.addCommand("coord", std::make_unique<CoordCommand>(sysman, taskSet));
	cli.addCommand("refresh", std::make_unique<RefreshCommand>(sysman, taskSet));
	cli.addCommand("task", std::make_unique<TaskCommand>(sysman, taskSet));
	cli.addCommand("help", std::make_unique<HelpCMD>());
	do {
		if (sysman.getDeviceManager().hasError()) {
			std::cout << "Errors occured during execution" << std::endl;
			while (sysman.getDeviceManager().hasError()) {
				std::cout << "Error: " << sysman.getDeviceManager().pollError()
				          << std::endl;
			}
		}
	} while (cli.shell());
	return EXIT_SUCCESS;
}
