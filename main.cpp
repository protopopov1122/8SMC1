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
		std::cout << "ls:\tList all connected devices" << std::endl;
		std::cout << "ls state $devid:\tList selected device state" << std::endl;
		std::cout << "power $devid:\tFlip power on selected device" << std::endl;
		std::cout << "revert $devid:\tSet start position of selected device to 0" << std::endl;
		std::cout << "pos $devid $value:\tSet current position of selected device" << std::endl;
		std::cout << "start $devid $dest [$speed [$step-divisor]]:\tStart selected device" << std::endl;
		std::cout << "stop $devid:\tStop selected device" << std::endl;
		std::cout << "x:\tStop last started device" << std::endl;
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
	cli.addCommand("start", new StartCommand(&devman));
	cli.addCommand("stop", new StopCommand(&devman));
	cli.addCommand("x", new StopLastCommand(&devman));
	cli.addCommand("mode", new ModeSetCommand(&devman));
	cli.addCommand("help", new HelpCMD());
	while (cli.shell()) {}
	return EXIT_SUCCESS;
}
