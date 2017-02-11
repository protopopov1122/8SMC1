#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <cinttypes>
#include "DevCLI.h"

/* Creates device manager and runs command line*/

using namespace Controller;

class EchoCMD : public CLICommand  {
	virtual ~EchoCMD() {

	}

	virtual void execute(std::vector<std::string> &args) {
		for (size_t i = 0; i < args.size(); i++) {
			std::cout << args.at(i) << " ";
		}
		std::cout << std::endl;
	}
};

class HelpCMD : public CLICommand  {
	virtual ~HelpCMD() {

	}

	virtual void execute(std::vector<std::string> &args) {
		std::cout << "Help command stub" << std::endl;
	}
};

int main(int argc, char **argv) {
	DeviceManager devman;
	CLI cli;
	cli.addCommand("echo", new EchoCMD());
	cli.addCommand("ls", new LsCommand(&devman));
	cli.addCommand("power", new PowerCommand(&devman));
	cli.addCommand("revert", new RevertStartCommand(&devman));
	cli.addCommand("pos", new PositionCommand(&devman));
	cli.addCommand("reset", new ResetPositionCommand(&devman));
	cli.addCommand("start", new StartCommand(&devman));
	cli.addCommand("stop", new StopCommand(&devman));
	cli.addCommand("x", new StopLastCommand(&devman));
	cli.addCommand("mode", new ModeSetCommand(&devman));
	cli.addCommand("roll", new RollCommand(&devman));
	cli.addCommand("coord", new CoordCommand(&devman));
	cli.addCommand("move", new MoveCommand(&devman));
	cli.addCommand("cal", new CalibrateCommand(&devman));
	cli.addCommand("refresh", new RefreshCommand(&devman));
	cli.addCommand("help", new HelpCMD());
	while (cli.shell()) {
		if (devman.hasError()) {
			std::cout << "Errors occured during execution" << std::endl;
			while (devman.hasError()) {
				std::cout << "Error: " << devman.pollError() << std::endl;
			}
		}
	}
	return EXIT_SUCCESS;
}
