#include "ui/CalxDebugConsole.h"
#include "cli/DevCLI.h"

namespace CalXUI {

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

	CalxDebugConsole::CalxDebugConsole(SystemManager *sysman)
	    : devman(sysman->getDeviceManager()) {
		this->sysman = sysman;
	}

	void *CalxDebugConsole::Entry() {
		CLI cli(std::cout, std::cin);
		cli.addCommand("echo", new EchoCMD());
		cli.addCommand("ls", new LsCommand(sysman, taskSet));
		cli.addCommand("x", new HaltCommand(sysman, taskSet));
		cli.addCommand("dev", new MotorCommand(sysman, taskSet));
		cli.addCommand("coord", new CoordCommand(sysman, taskSet));
		cli.addCommand("refresh", new RefreshCommand(sysman, taskSet));
		cli.addCommand("task", new TaskCommand(sysman, taskSet));
		cli.addCommand("help", new HelpCMD());
		do {
			if (devman.hasError()) {
				std::cout << "Errors occured during execution" << std::endl;
				while (devman.hasError()) {
					std::cout << "Error: " << devman.pollError() << std::endl;
				}
			}
		} while (cli.shell());
		return nullptr;
	}
}  // namespace CalXUI
