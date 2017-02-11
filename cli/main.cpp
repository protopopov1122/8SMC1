#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <cinttypes>
#include "SystemManager.h"
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
	SystemManager sysman(&devman);
	CLI cli;
	cli.addCommand("echo", new EchoCMD());
	cli.addCommand("ls", new LsCommand(&sysman));
	cli.addCommand("power", new PowerCommand(&sysman));
	cli.addCommand("revert", new RevertStartCommand(&sysman));
	cli.addCommand("pos", new PositionCommand(&sysman));
	cli.addCommand("reset", new ResetPositionCommand(&sysman));
	cli.addCommand("start", new StartCommand(&sysman));
	cli.addCommand("stop", new StopCommand(&sysman));
	cli.addCommand("x", new StopLastCommand(&sysman));
	cli.addCommand("mode", new ModeSetCommand(&sysman));
	cli.addCommand("roll", new RollCommand(&sysman));
	cli.addCommand("coord", new CoordCommand(&sysman));
	cli.addCommand("move", new MoveCommand(&sysman));
	cli.addCommand("cal", new CalibrateCommand(&sysman));
	cli.addCommand("refresh", new RefreshCommand(&sysman));
	cli.addCommand("task", new TaskCommand(&sysman));
	cli.addCommand("+move", new LinearCommand(&sysman));
	cli.addCommand("+cal", new CalibrateStepCommand(&sysman));
	cli.addCommand("exec", new ExecuteCommand(&sysman));
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
