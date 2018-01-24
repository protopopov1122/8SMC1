/*
        Copyright (c) 2017 Jevgenijs Protopopovs

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

#include "cli/DevCLI.h"
#include "ctrl-lib/SignalHandler.h"
#include "ctrl-lib/SystemManager.h"
#include "ctrl-lib/device/DeviceManager.h"
#include "ctrl-lib/translator/CoordTranslator.h"
#include <cinttypes>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

/* Creates device manager and runs command line*/

using namespace CalX;

class EchoCMD : public CLICommand {
	virtual ~EchoCMD() {}

	virtual void execute(CLI *cli, std::vector<std::string> &args) {
		for (size_t i = 0; i < args.size(); i++) {
			std::cout << args.at(i) << " ";
		}
		std::cout << std::endl;
	}
};

class HelpCMD : public CLICommand {
	virtual ~HelpCMD() {}

	virtual void execute(CLI *cli, std::vector<std::string> &args) {
		std::cout << "See cli/README.md in project repo to get CLI manual"
		          << std::endl;
	}
};

int main(int argc, char **argv) {
	std::unique_ptr<DeviceManager> devman =
	    std::unique_ptr<DeviceManager>(getDeviceManager());
	std::shared_ptr<ConfigManager> conf = nullptr;
	std::ifstream cnf("main.conf.ini");
	if (!cnf.good()) {
		std::cout << "Can't load configuration, using default values." << std::endl;
		conf = std::make_shared<ConfigManager>();
	} else {
		conf = ConfigManager::load(cnf, std::cout);
	}
	cnf.close();
	SystemManager *sysman = new SystemManager(std::move(devman), conf);
	setup_signals(sysman);
	CLI cli(std::cout, std::cin);
	cli.addCommand("echo", new EchoCMD());
	cli.addCommand("ls", new LsCommand(sysman));
	cli.addCommand("x", new HaltCommand(sysman));
	cli.addCommand("dev", new MotorCommand(sysman));
	cli.addCommand("coord", new CoordCommand(sysman));
	cli.addCommand("refresh", new RefreshCommand(sysman));
	cli.addCommand("task", new TaskCommand(sysman));
	cli.addCommand("help", new HelpCMD());
	do {
		if (sysman->getDeviceManager().hasError()) {
			std::cout << "Errors occured during execution" << std::endl;
			while (sysman->getDeviceManager().hasError()) {
				std::cout << "Error: " << sysman->getDeviceManager().pollError()
				          << std::endl;
			}
		}
	} while (cli.shell());
	delete sysman;
	return EXIT_SUCCESS;
}
