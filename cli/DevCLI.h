#ifndef _8SMC1_DEVCLI_H_
#define _8SMC1_DEVCLI_H_

#include "CLI.h"
#include "SystemManager.h"

/* Defines cli commands to control Devices API */

namespace Controller {
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
	CMD(PowerCommand)
	CMD(RevertStartCommand)
	CMD(SaveCommand)
	CMD(PositionCommand)
	CMD(ResetPositionCommand)
	CMD(StartCommand)
	CMD(StopCommand)
	CMD(StopLastCommand)
	CMD(ModeSetCommand)
	CMD(RollCommand)
	CMD(CoordCommand)
	CMD(MoveCommand)
	CMD(CalibrateCommand)
	CMD(RefreshCommand)
	CMD(TaskCommand)
	CMD(LinearCommand)
	CMD(CalibrateStepCommand)
	CMD(ExecuteCommand)

	#undef CMD
}

#endif
