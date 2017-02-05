#include <stdio.h>
#include <iostream>
#include "DevCLI.h"

namespace Controller {
	void LsCommand::execute(std::vector<std::string> &args) {
		if (args.empty()) {	// For empty args list all devices
			for (unsigned int i = 0; i < devman->getDeviceCount(); i++) {
				Device *dev = devman->getDevice(i);
				std::cout << "Device - " << dev->getID() << "; "
					<< "Serial Number - " << dev->getSerial() << "; "
					<< "Version - " << dev->getVersion() << std::endl;
			}
		} else {
			std::string req = args.at(0);
			const std::string STATE = "state";
			const std::string MODE = "mode";
			#define cmp(str) (req.length() == str.length() && req.compare(0, str.length(), str) == 0)
			if (cmp(STATE)) { // View device state
				if (args.size() < 2) {
					std::cout << "Provide device id" << std::endl;
					return;
				}
				Device *dev = devman->getDevice(std::stoi(args.at(1)));
				if (dev == nullptr) {
					std::cout << "Device not found" << std::endl;
					return;
				}
				std::cout << "Device #" << dev->getID() << " state" << std::endl;
				std::cout << "\tCurrent position\t" << dev->getPosition() << std::endl;
				std::cout << "\tTemperature\t" << dev->getTemperature() << std::endl;
				std::cout << "\tVoltage\t" << dev->getVoltage() << std::endl;
				std::cout << "\tStep divisor\t" << (int) dev->getStepDivisor() << std::endl;
				std::cout << "\tPower\t" << (dev->getPowerState() == Power::FullPower ?
								"full" : (dev->getPowerState() == Power::HalfPower ?
								"half" : "no")) << std::endl;
				std::cout << "\tTemperature\t" << dev->getTemperature() << std::endl;
				std::cout << "\tTrailer 1\t" << (dev->isTrailerPressed(1) ? "Pushed" : "Unpushed") << std::endl;
				std::cout << "\tTrailer 2\t" << (dev->isTrailerPressed(2) ? "Pushed" : "Unpushed") << std::endl;
				std::cout << "\tInput synchronization\t" << (dev->getInputSync() ? "True" : "False") << std::endl;
				std::cout << "\tOutput synchronization\t" << (dev->getOutputSync() ? "True" : "False") << std::endl;
				std::cout << (dev->isRunning() ? "\tRunning" : "\tNot running") << (dev->isRunning() &&
						dev->hasFullSpeed() ? " at full speed" : "") << std::endl;
				if (dev->isAfterReset()) {
					std::cout << "\tDevice is after reset" << std::endl;
				}
			} else if (cmp(MODE)) { // View device mode parameters
				if (args.size() < 2) {
					std::cout << "Provide device id" << std::endl;
					return;
				}
				Device *dev = devman->getDevice(std::stoi(args.at(1)));
				if (dev == nullptr) {
					std::cout << "Device not found" << std::endl;
					return;
				}
				#define PRINT(nm, meth) std::cout << "\t" << nm << "\t" <<\
							(dev->meth() ? "true" : "false") << std::endl;
				PRINT("output-sync-enabled", isOutputSyncEnabled)
				PRINT("trailer1-enabled", isTrailer1Enabled)
				PRINT("trailer2-enabled", isTrailer2Enabled)
				PRINT("trailer-swapped", isTrailerSwapped)
				PRINT("sync-input-mode", getSyncInputMode)
				PRINT("buttons-diabled", isButtonDisabled)
				PRINT("trailer1-true-state", getTrailer1TrueState)
				PRINT("trailer2-true-state", getTrailer2TrueState)
				#undef PRINT
			} else {
				std::cout << "Unknown parameter '" << req << "'" << std::endl;
			}
			#undef cmp
		}
	}

	void PowerCommand::execute(std::vector<std::string> &args) {
		if (args.empty()) {
			std::cout << "Provide device id" << std::endl;
		} else {
			Device *dev = devman->getDevice(std::stoi(args.at(0)));
			if (dev == nullptr) {
				std::cout << "Device not found" << std::endl;
				return;
			}
			if (dev->flipPower()) {
				std::cout << "\tPower\t" << (dev->getPowerState() == Power::FullPower ?
								"full" : (dev->getPowerState() == Power::HalfPower ?
								"half" : "no")) << std::endl;
			}
		}
	}

	void RevertStartCommand::execute(std::vector<std::string> &args) {
		if (args.empty()) {
			std::cout << "Provide device id" << std::endl;
		} else {
			Device *dev = devman->getDevice(std::stoi(args.at(0)));
			if (dev == nullptr) {
				std::cout << "Device not found" << std::endl;
				return;
			}
			if (dev->revertStart()) {
				std::cout << "\tOk" << std::endl;
			}
		}
	}

	void SaveCommand::execute(std::vector<std::string> &args) {
		if (args.empty()) {
			std::cout << "Provide device id" << std::endl;
		} else {
			Device *dev = devman->getDevice(std::stoi(args.at(0)));
			if (dev == nullptr) {
				std::cout << "Device not found" << std::endl;
				return;
			}
			if (dev->saveToFlash()) {
				std::cout << "\tOk" << std::endl;
			}
		}
	}

	void PositionCommand::execute(std::vector<std::string> &args) {
		if (args.empty()) {
			std::cout << "Provide device id" << std::endl;
		} else if (args.size() == 1) {
			std::cout << "Provide current position" << std::endl;
		} else {
			Device *dev = devman->getDevice(std::stoi(args.at(0)));
			if (dev == nullptr) {
				std::cout << "Device not found" << std::endl;
				return;
			}
			int pos = std::stoi(args.at(1));
			if (dev->setCurrentPosition(pos)) {
				std::cout << "\tOk" << std::endl;
			}
		}
	}

	void StartCommand::execute(std::vector<std::string> &args) {
		if (args.empty()) {
			std::cout << "Provide device id" << std::endl;
		} else {
			if (args.size() < 2) {
				std::cout << "Provide destination" << std::endl;
				return;
			}
			Device *dev = devman->getDevice(std::stoi(args.at(0)));
			if (dev == nullptr) {
				std::cout << "Device not found" << std::endl;
				return;
			}
			int dest = std::stoi(args.at(1));
			float speed = dev->getSpeed();
			int div = dev->getStepDivisor();
			switch (args.size()) {
				case 3: {
					speed = std::stod(args.at(2));
				} break;
				case 4: {
					float speed = std::stod(args.at(2));
					int div = std::stoi(args.at(3));
				} break;
			}
			if (dev->start(dest, speed, div)) {
				std::cout << "\tStarted device #" << dev->getID() << " to " << dest
					<< " with speed " << speed << " steps/sec and " << div << " step divisor"
					<< std::endl;
			}
		}
	}

	void StopCommand::execute(std::vector<std::string> &args) {
		if (args.empty()) {
			std::cout << "Provide device id" << std::endl;
		} else {
			Device *dev = devman->getDevice(std::stoi(args.at(0)));
			if (dev == nullptr) {
				std::cout << "Device not found" << std::endl;
				return;
			}
			if (dev->stop()) {
				std::cout << "\tStopped device #" << dev->getID() << std::endl;
			}
		}
	}

	void StopLastCommand::execute(std::vector<std::string> &args) {
		Device *dev = devman->getLastDevice();
		if (dev == nullptr) {
			std::cout << "No device was used" << std::endl;
			return;
		}
		if (dev->stop()) {
			std::cout << "\tOk" << std::endl;
		}
	}

	void ModeSetCommand::execute(std::vector<std::string> &args) {
		if (args.empty()) {
			std::cout << "Provide device id" << std::endl;
			return;
		}
		if (args.size() < 3) {
			std::cout << "Provide parameter name and value" << std::endl;
			return;
		}
		Device *dev = devman->getDevice(std::stoi(args.at(0)));
		if (dev == nullptr) {
			std::cout << "Device not found" << std::endl;
			return;
		}
		std::string name = args.at(1);
		bool value = args.at(2).compare("true") == 0;
		#define PAR(nm, meth) if (name.length() == strlen(nm) &&\
					name.compare(nm) == 0) {\
			dev->meth(value);\
		}
		#define EPAR(nm, meth) else PAR(nm, meth)
		PAR("output-sync-enabled", setOutputSyncEnabled)
		EPAR("trailer1-enabled", setTrailer1Enabled)
		EPAR("trailer2-enabled", setTrailer2Enabled)
		EPAR("trailer-swapped", setTrailerSwapped)
		EPAR("sync-input-mode", setSyncInputMode)
		EPAR("buttons-diabled", setButtonDisabled)
		EPAR("trailer1-true-state", setTrailer1TrueState)
		EPAR("trailer2-true-state", setTrailer2TrueState)
		else {
			std::cout << "Unknown parameter '" << name << "'" << std::endl;
			return;
		}
		#undef EPAR
		#undef PAR
		std::cout << "Parameter '" << name << "' set to " << (value ? "true" : "false") << std::endl;
	}
}
