#include <stdio.h>
#include <iostream>
#include "DevCLI.h"
#include "CircleGenerator.h"

namespace _8SMC1 {
	void LsCommand::execute(std::vector<std::string> &args) {
		if (args.empty()) {	// For empty args list all devices
			for (unsigned int i = 0; i < sysman->getDeviceManager()->getDeviceCount(); i++) {
				Device *dev = sysman->getDeviceManager()->getDevice(i);
				std::cout << "Device - " << dev->getID() << "; "
					<< "Serial Number - " << dev->getSerial() << "; "
					<< "Version - " << dev->getVersion() << std::endl;
			}
		} else {
			std::string req = args.at(0);
			const std::string STATE = "state";
			const std::string MODE = "mode";
			const std::string COORDS = "coords";
			const std::string TASKS = "tasks";
			#define cmp(str) (req.length() == str.length() && req.compare(0, str.length(), str) == 0)
			if (cmp(STATE)) { // View device state
				if (args.size() < 2) {
					std::cout << "Provide device id" << std::endl;
					return;
				}
				Device *dev = sysman->getDeviceManager()->getDevice(std::stoi(args.at(1)));
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
				Device *dev = sysman->getDeviceManager()->getDevice(std::stoi(args.at(1)));
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
			} else if (cmp(COORDS)) {
				for (size_t i = 0; i < sysman->getCoordCount(); i++) {
					CoordController *ctrl = sysman->getCoord(i);
					std::cout << i << "\tdev: " << ctrl->getXAxis()->getDevice()->getID()
						<< "; dev: " << ctrl->getYAxis()->getDevice()->getID() << std::endl;
				}
			} else if (cmp(TASKS)) {
				for (size_t i = 0; i < sysman->getTaskCount(); i++) {
					CoordTask *task = sysman->getTask(i);
					std::cout << i << "\tsize: " << task->getSubCount() << std::endl;
				}
			} else {
				std::cout << "Unknown parameter '" << req << "'" << std::endl;
			}
			#undef cmp
		}
	}

	void HaltCommand::execute(std::vector<std::string> &args) {
		DeviceManager *devman = sysman->getDeviceManager();
		for (size_t i = 0; i < devman->getDeviceCount(); i++) {
			Device *dev = devman->getDevice(i);
			dev->stop();
		}
	}

	void MotorCommand::execute(std::vector<std::string> &args) {
		if (args.empty()) {
			std::cout << "Provide arguments" << std::endl;
			return;
		}
		std::string com = args.at(0);
		args.erase(args.begin());
		if (com.compare("roll") == 0) {
			if (args.empty()) {
				std::cout << "Provide device id" << std::endl;
			} else {
				if (args.size() < 2) {
					std::cout << "Provide trailer id" << std::endl;
					return;
				}
				DeviceController *dev = sysman->getDeviceController(std::stoi(args.at(0)));
				if (dev == nullptr) {
					std::cout << "Device not found" << std::endl;
					return;
				}
				int dest = std::stoi(args.at(1));
				dev->moveToTrailer(dest, 800);
			}
		} else if (com.compare("start") == 0) {	
			if (args.empty()) {
				std::cout << "Provide device id" << std::endl;
			} else {
				if (args.size() < 2) {
					std::cout << "Provide destination" << std::endl;
					return;
				}
				DeviceController *dev = sysman->getDeviceController(std::stoi(args.at(0)));
				if (dev == nullptr) {
					std::cout << "Device not found" << std::endl;
					return;
				}
				int dest = std::stoi(args.at(1));
				float speed = dev->getDevice()->getSpeed();
				int div = dev->getDevice()->getStepDivisor();
				switch (args.size()) {
					case 3: {
						speed = std::stod(args.at(2));
					} break;
					case 4: {
						speed = std::stod(args.at(2));
						div = std::stoi(args.at(3));
					} break;
				}
				std::cout << "\tStarted device #" << dev->getDevice()->getID() << " to " << dest
					<< " with speed " << speed << " steps/sec and " << div << " step divisor"
					<< std::endl;
				dev->startMove(dest, speed, div);
			}
		} else if (com.compare("stop") == 0) {
			if (args.empty()) {
				std::cout << "Provide device id" << std::endl;
			} else {
				DeviceController *dev = sysman->getDeviceController(std::stoi(args.at(0)));
				if (dev == nullptr) {
					std::cout << "Device not found" << std::endl;
					return;
				}
				dev->stop();
				std::cout << "\tStopped device #" << dev->getDevice()->getID() << std::endl;
			}
		} else if (com.compare("power") == 0) {
			if (args.empty()) {
				std::cout << "Provide device id" << std::endl;
			} else for (size_t i = 0; i < args.size(); i++) {
				Device *dev = sysman->getDeviceManager()->getDevice(std::stoi(args.at(i)));
				if (dev == nullptr) {
					std::cout << "Device #" << i << " not found" << std::endl;
					return;
				}
				if (dev->flipPower()) {
					std::cout << "\tDevice #" << i << ": power\t" << (dev->getPowerState() == Power::FullPower ?
									"full" : (dev->getPowerState() == Power::HalfPower ?
									"half" : "no")) << std::endl;
				}
			}
		} else if (com.compare("mode") == 0) {	
			if (args.empty()) {
				std::cout << "Provide device id" << std::endl;
				return;
			}
			if (args.size() < 3) {
				std::cout << "Provide parameter name and value" << std::endl;
				return;
			}
			Device *dev = sysman->getDeviceManager()->getDevice(std::stoi(args.at(0)));
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
		} else if (com.compare("reset") == 0) {
			if (args.empty()) {
				std::cout << "Provide device id" << std::endl;
			} else {
				DeviceController *dev = sysman->getDeviceController(std::stoi(args.at(0)));
				if (dev == nullptr) {
					std::cout << "Device not found" << std::endl;
					return;
				}
				if (dev->resetPosition() == ErrorCode::NoError) {
					std::cout << "\tOk" << std::endl;
				}
			}
		} else {
			std::cout << "Wrong command '" << com << "'" << std::endl;
		}
	}

	void CoordCommand::execute(std::vector<std::string> &args) {
		if (args.empty()) {
			std::cout << "Provide arguments" << std::endl;
		}
		std::string com = args.at(0);
		args.erase(args.begin());
		if (com.compare("new") == 0) {
			if (args.size() < 2) {
				std::cout << "Provide devices" << std::endl;
			} else {
				DWORD d1 = std::stoi(args.at(0));
				DWORD d2 = std::stoi(args.at(1));
				if (sysman->createCoord(d1, d2) == nullptr) {
					std::cout << "Wrong device ids" << std::endl;
				} else {
					std::cout << "Created coord #" << sysman->getCoordCount() - 1 << std::endl;
				}
			}
		} else if (com.compare("move") == 0) {
			if (args.size() < 5) {
				std::cout << "Provide arguments" << std::endl;
			} else {
				CoordController *ctrl = sysman->getCoord(std::stoi(args.at(0)));
				int x = std::stoi(args.at(1));
				int y = std::stoi(args.at(2));
				float speed = std::stod(args.at(3));
				int div = std::stod(args.at(4));
				if (ctrl == nullptr) {
					std::cout << "Wrong coord id" << std::endl;
				} else {
					motor_point_t point = {x, y};
					std::cout << "\tStarted coord #" << args.at(0) << " to " << x << "x" << y
						<< " with base speed " << speed << " steps/sec and " << div << " step divisor"
						<< std::endl;
					ctrl->move(point, speed, div, true);
				}
			}
		} else if (com.compare("jump") == 0) {
			if (args.size() < 5) {
				std::cout << "Provide arguments" << std::endl;
			} else {
				CoordController *ctrl = sysman->getCoord(std::stoi(args.at(0)));
				int x = std::stoi(args.at(1));
				int y = std::stoi(args.at(2));
				float speed = std::stod(args.at(3));
				int div = std::stod(args.at(4));
				if (ctrl == nullptr) {
					std::cout << "Wrong coord id" << std::endl;
				} else {
					motor_point_t point = {x, y};
					std::cout << "\tStarted coord #" << args.at(0) << " to " << x << "x" << y
						<< " with base speed " << speed << " steps/sec and " << div << " step divisor"
						<< std::endl;
					ctrl->move(point, speed, div, false);
				}
			}
		} else if (com.compare("cal") == 0) {
			if (args.size() < 2) {
				std::cout << "Provide arguments" << std::endl;
				return;
			}
			int coordNum = std::stoi(args.at(0));
			int coordTrailer = std::stoi(args.at(1));
			CoordController *coordController = sysman->getCoord(coordNum);
			if (coordController == nullptr) {
				std::cout << "Wrong coord id" << std::endl;
			}
			coordController->calibrate(coordTrailer);
		} else {
			std::cout << "Wrong command '" << com << "'" << std::endl;
		}
	}
	
	void RefreshCommand::execute(std::vector<std::string> &args) {
		sysman->getDeviceManager()->refresh();
	}

	void TaskCommand::execute(std::vector<std::string> &args) {
		if (args.empty()) {
			std::cout << "Provide parameter" << std::endl;
			return;
		}
		if (args.at(0).compare("new") == 0) {
			CoordTask *task = sysman->createTask();
			if (task == nullptr) {
				std::cout << "Error occured" << std::endl;
			} else {
				std::cout << "Created task #" << (this->sysman->getTaskCount() - 1) << std::endl;
			}
		} else if (args.at(0).compare("rm") == 0) {
			if (args.size() < 2) {
				std::cout << "Provide task id" << std::endl;
				return;
			}
			if (!sysman->removeTask(std::stoi(args.at(1)))) {
				std::cout << "Wrong task id" << std::endl;
			} else {
				std::cout << "Ok" << std::endl;
			}
		} else if (args.at(0).compare("add") == 0) {
			args.erase(args.begin());
			CoordTask *task = sysman->getTask(std::stoi(args.at(0)));
			args.erase(args.begin());
			std::string com = args.at(0);
			args.erase(args.begin());
			if (task == nullptr) {
				std::cout << "Wrong task id" << std::endl;
				return;
			}
			if (com.compare("move") == 0) {
				if (args.size() < 3) {
					std::cout << "Wrong argument count" << std::endl;
					return;
				}
				int x = std::stoi(args.at(0));
				int y = std::stoi(args.at(1));
				float sp = std::stod(args.at(2));
				if (sp <= 0 || sp > 1) {
					std::cout << "Wrong speed coef" << std::endl;
					return;
				}
				motor_point_t pnt = {x, y};
				task->addStep(new MoveTaskStep(pnt, sp));
			} else if (com.compare("jump") == 0) {
				if (args.size() < 3) {
					std::cout << "Wrong argument count" << std::endl;
					return;
				}
				int x = std::stoi(args.at(0));
				int y = std::stoi(args.at(1));
				float sp = std::stod(args.at(2));
				if (sp <= 0 || sp > 1) {
					std::cout << "Wrong speed coef" << std::endl;
					return;
				}
				motor_point_t pnt = {x, y};
				task->addStep(new JumpTaskStep(pnt, sp));
			} else if (com.compare("arc") == 0) {
				if (args.size() < 6) {
					std::cout << "Wrong argument count" << std::endl;
					return;
				}
				int x = std::stoi(args.at(0));
				int y = std::stoi(args.at(1));
				int cx = std::stoi(args.at(2));
				int cy = std::stoi(args.at(3));
				int sp = std::stoi(args.at(4));
				float speed = std::stod(args.at(5));
				if (speed <= 0 || speed > 1) {
					std::cout << "Wrong speed coef" << std::endl;
					return;
				}
				motor_point_t pnt = {x, y};
				motor_point_t cpnt = {cx, cy};
				task->addStep(new ArcTaskStep(pnt, cpnt, sp, speed));
			} else if (com.compare("carc") == 0) {
				if (args.size() < 6) {
					std::cout << "Wrong argument count" << std::endl;
					return;
				}
				int x = std::stoi(args.at(0));
				int y = std::stoi(args.at(1));
				int cx = std::stoi(args.at(2));
				int cy = std::stoi(args.at(3));
				int sp = std::stoi(args.at(4));
				float speed = std::stod(args.at(5));
				if (speed <= 0 || speed > 1) {
					std::cout << "Wrong speed coef" << std::endl;
					return;
				}
				motor_point_t pnt = {x, y};
				motor_point_t cpnt = {cx, cy};
				ArcTaskStep *st = new ArcTaskStep(pnt, cpnt, sp, speed);
				st->setClockwise(false);
				task->addStep(st);
			} else if (com.compare("cal") == 0) {
				if (args.empty()) {
					std::cout << "Wrong argument count" << std::endl;
					return;
				}
				int side = std::stoi(args.at(0));
				if (side != 1 && side != 2) {
					std::cout << "Wrond calibration side" << std::endl;
					return;
				}
				task->addStep(new CalibrateTaskStep(side));
			} else {
				std::cout << "Wrong command '" << com << "'" << std::endl;
			}
		} else if (args.at(0).compare("exec") == 0) {
			if (args.size() < 4) {
				std::cout << "Wrong argument count" << std::endl;
				return;
			}
			CoordTask *task = sysman->getTask(std::stoi(args.at(1)));
			CoordController *coord = sysman->getCoord(std::stoi(args.at(2)));
			float speed = std::stod(args.at(3));
			if (task == nullptr) {
				std::cout << "Wrong task id" << std::endl;
				return;
			}
			if (coord == nullptr) {
				std::cout << "Wrong coord id" << std::endl;
				return;
			}
			TaskParameters prms = {speed};
			task->perform(coord, prms);
		} else {
			std::cout << "Wrong command '" << args.at(0) << "'" << std::endl;
		}
	}

	void CircleCommand::execute(std::vector<std::string> &args) {
		if (args.size() < 3) {
			std::cout << "Provide arguments" << std::endl;
			return;
		}
		int x = std::stoi(args.at(0));
		int y = std::stoi(args.at(1));
		int rad = std::stoi(args.at(2));
		motor_point_t center = {x, y};
		Circle cir(center, rad);
		for (size_t i = 0; i < cir.getFullSize(); i++) {
			motor_point_t pnt = cir.getElement(i);
			std::cout << pnt.x << "x" << pnt.y << std::endl;
		}
	}
}
