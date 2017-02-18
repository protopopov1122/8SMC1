#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <cinttypes>
#include "SystemManager.h"
#include "DevCLI.h"

/* Creates device manager and runs command line*/

using namespace _8SMC1;

class EchoCMD : public CLICommand  {
	virtual ~EchoCMD() {

	}

	virtual void execute(std::vector<std::string> &args) {
	//	for (size_t i = 0; i < args.size(); i++) {
		//	std::cout << args.at(i) << " ";
		//}
		std::cout << args.at(0) << std::endl;
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
	cli.addCommand("x", new HaltCommand(&sysman));
	cli.addCommand("dev", new MotorCommand(&sysman));
	cli.addCommand("coord", new CoordCommand(&sysman));
	cli.addCommand("refresh", new RefreshCommand(&sysman));
	cli.addCommand("task", new TaskCommand(&sysman));
	cli.addCommand("cir", new CircleCommand(&sysman));
	cli.addCommand("help", new HelpCMD());
	do {
		if (devman.hasError()) {
			std::cout << "Errors occured during execution" << std::endl;
			while (devman.hasError()) {
				std::cout << "Error: " << devman.pollError() << std::endl;
			}
		}
	} while (cli.shell());
	return EXIT_SUCCESS;
}
