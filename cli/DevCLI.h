#ifndef CALX_DEVCLI_H_
#define CALX_DEVCLI_H_

#include "CLI.h"
#include "ctrl-lib/SystemManager.h"
#include "ctrl-lib/misc/GCodeParser.h"

/* Command line commands are used to test system functionality*
   while GUI is not developed. Most commands are directly mapped
   to system functions, most of them has huge amount of parameters,
   see README.md for quick tutorial and DevCLI.cpp for implemetation.
   This commands use abstract CLI interface.*/

namespace CalX {
	class DeviceCommand : public CLICommand {
		public:
			DeviceCommand(SystemManager *sysman) {
				this->sysman = sysman;
			}
			virtual ~DeviceCommand() {}
			virtual void execute(CLI *cli, std::vector<std::string> &args) = 0;
		protected:
			SystemManager *sysman;
	};
	
	#define CMD(name) class name : public DeviceCommand {\
		public:\
			name(SystemManager *sysman) :\
				DeviceCommand::DeviceCommand(sysman) {};\
			virtual ~name() {};\
			virtual void execute(CLI *cli, std::vector<std::string> &args);\
	};

	CMD(LsCommand)
	CMD(HaltCommand)
	CMD(CoordCommand)
	CMD(RefreshCommand)
	CMD(TaskCommand)
	CMD(MotorCommand)

	#undef CMD
}

#endif
