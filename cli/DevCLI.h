#ifndef _8SMC1_DEVCLI_H_
#define _8SMC1_DEVCLI_H_

#include "CLI.h"
#include "DeviceController.h"

/* Defines cli commands to control Devices API */

namespace Controller {
	class DeviceCommand : public CLICommand {
		public:
			DeviceCommand(DeviceManager *devman) {
				this->devman = devman;
			}
			virtual ~DeviceCommand() {}
			virtual void execute(std::vector<std::string> &args) = 0;
		protected:
			DeviceManager *devman;
	};
	
	#define CMD(name) class name : public DeviceCommand {\
		public:\
			name(DeviceManager *devman) :\
				DeviceCommand::DeviceCommand(devman) {};\
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

	#undef CMD
}

#endif
