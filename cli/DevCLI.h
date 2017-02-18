#ifndef _8SMC1_DEVCLI_H_
#define _8SMC1_DEVCLI_H_

#include "CLI.h"
#include "SystemManager.h"
#include "GCodeParser.h"

/* Defines cli commands to control Devices API */

namespace _8SMC1 {
	class DeviceCommand : public CLICommand {
		public:
			DeviceCommand(SystemManager *sysman) {
				this->sysman = sysman;
			}
			virtual ~DeviceCommand() {}
			virtual void execute(std::vector<std::string> &args) = 0;
		protected:
			SystemManager *sysman;
	};
	
	#define CMD(name) class name : public DeviceCommand {\
		public:\
			name(SystemManager *sysman) :\
				DeviceCommand::DeviceCommand(sysman) {};\
			virtual ~name() {};\
			virtual void execute(std::vector<std::string> &args);\
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
