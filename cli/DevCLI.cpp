#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "DevCLI.h"
#include "ctrl-lib/misc/CircleGenerator.h"
#include "ctrl-lib/misc/GCodeParser.h"
#include "ctrl-lib/misc/GraphBuilder.h"
#include "ctrl-lib/graph/FunctionParser.h"
#include "ctrl-lib/graph/FunctionEngine.h"

namespace _8SMC1 {
	void LsCommand::execute(CLI *cli, std::vector<std::string> &args) {
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
					std::cout << i;
					sysman->getCoord(i)->dump(std::cout);
					std::cout << std::endl;
				}
			} else if (cmp(TASKS)) {
				for (size_t i = 0; i < sysman->getTaskCount(); i++) {
					CoordTask *task = sysman->getTask(i);
					std::cout << i;
					if (task->getType() == CoordTaskType::ProgrammedTask) {
						ProgrammedCoordTask *pct = (ProgrammedCoordTask*) task;
						std::cout << "\tsize: " << pct->getSubCount();
					}
					std::cout << std::endl;
				}
			} else {
				std::cout << "Unknown parameter '" << req << "'" << std::endl;
			}
			#undef cmp
		}
	}

	void HaltCommand::execute(CLI *cli, std::vector<std::string> &args) {
		DeviceManager *devman = sysman->getDeviceManager();
		for (size_t i = 0; i < devman->getDeviceCount(); i++) {
			Device *dev = devman->getDevice(i);
			dev->stop();
		}
	}

	void MotorCommand::execute(CLI *cli, std::vector<std::string> &args) {
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
		} else if (com.compare("move") == 0) {	
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
		} else if (com.compare("rmove") == 0) {	
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
				dev->startRelativeMove(dest, speed, div);
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

	void CoordCommand::execute(CLI *cli, std::vector<std::string> &args) {
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
		} else if (com.compare("pop") == 0) {
			if (args.empty()) {
				std::cout << "Provide arguments" << std::endl;
				return;
			}
			CoordPlaneStack *ctrl = sysman->getCoord(std::stoi(args.at(0)));
			if (ctrl->popPlane()) {
				std::cout << "\tOk" << std::endl;
			} else {
				std::cout << "\tFailed" << std::endl;
			}
		} else if (com.compare("log") == 0) {
			if (args.empty()) {
				std::cout << "Provide coord id" << std::endl;
				return;
			}
			CoordPlaneStack *ctrl = sysman->getCoord(std::stoi(args.at(0)));
			if (ctrl == nullptr) {
				std::cout << "Wrong coord id" << std::endl;
				return;
			}
			std::string prefix = "";
			for (size_t i = 1; i < args.size(); i++) {
				prefix += args.at(i);
				if (i + 1 < args.size()) {
					prefix += " ";
				}
			}
			ctrl->pushPlane(new CoordPlaneLog(ctrl->peekPlane(), std::cout, prefix));
		} else if (com.compare("map") == 0) {
			if (args.size() < 5) {
				std::cout << "Provide arguments" << std::endl;
				return;
			}
			CoordPlaneStack *ctrl = sysman->getCoord(std::stoi(args.at(0)));
			if (ctrl == nullptr) {
				std::cout << "Wrong coord id" << std::endl;
				return;
			}
			motor_point_t offset = {std::stoi(args.at(1)), std::stoi(args.at(2))};
			motor_scale_t scale = {std::stod(args.at(3)), std::stod(args.at(4))};
			ctrl->pushPlane(new CoordPlaneMap(offset, scale, ctrl->peekPlane()));
		} else if (com.compare("logmap") == 0) {
			if (args.size() < 3) {
				std::cout << "Provide arguments" << std::endl;
				return;
			}
			CoordPlaneStack *ctrl = sysman->getCoord(std::stoi(args.at(0)));
			if (ctrl == nullptr) {
				std::cout << "Wrong coord id" << std::endl;
				return;
			}
			motor_scale_t scale = {std::stod(args.at(1)), std::stod(args.at(2))};
			motor_scale_t logscale = {std::stod(args.at(3)), std::stod(args.at(4))};
			ctrl->pushPlane(new CoordPlaneMapLog(logscale, scale, ctrl->peekPlane()));
		} else if (com.compare("validate") == 0) {
			if (args.size() < 6) {
				std::cout << "Provide arguments" << std::endl;
				return;
			}
			CoordPlaneStack *ctrl = sysman->getCoord(std::stoi(args.at(0)));
			if (ctrl == nullptr) {
				std::cout << "Wrong coord id" << std::endl;
				return;
			}
			motor_point_t min = {std::stoi(args.at(1)), std::stoi(args.at(2))};
			motor_point_t max = {std::stoi(args.at(3)), std::stoi(args.at(4))};
			float speed = std::stod(args.at(5));
			ctrl->pushPlane(new CoordPlaneValidator(min, max, speed, ctrl->peekPlane()));
		} else if (com.compare("move") == 0) {
			if (args.size() < 5) {
				std::cout << "Provide arguments" << std::endl;
			} else {
				CoordPlaneStack *ctrl = sysman->getCoord(std::stoi(args.at(0)));
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
		} else if (com.compare("rmove") == 0) {
			if (args.size() < 5) {
				std::cout << "Provide arguments" << std::endl;
			} else {
				CoordPlaneStack *ctrl = sysman->getCoord(std::stoi(args.at(0)));
				int x = std::stoi(args.at(1));
				int y = std::stoi(args.at(2));
				float speed = std::stod(args.at(3));
				int div = std::stod(args.at(4));
				if (ctrl == nullptr) {
					std::cout << "Wrong coord id" << std::endl;
				} else {
					motor_point_t point = {x, y};
					std::cout << "\tStarted relative coord #" << args.at(0) << " to " << x << "x" << y
						<< " with base speed " << speed << " steps/sec and " << div << " step divisor"
						<< std::endl;
					ctrl->relativeMove(point, speed, div, true);
				}
			}
		} else if (com.compare("jump") == 0) {
			if (args.size() < 5) {
				std::cout << "Provide arguments" << std::endl;
			} else {
				CoordPlaneStack *ctrl = sysman->getCoord(std::stoi(args.at(0)));
				int x = std::stoi(args.at(1));
				int y = std::stoi(args.at(2));
				float speed = std::stod(args.at(3));
				int div = std::stod(args.at(4));
				if (ctrl == nullptr) {
					std::cout << "Wrong coord id" << std::endl;
				} else {
					motor_point_t point = {x, y};
					std::cout << "\tStarted coord # jump " << args.at(0) << " to " << x << "x" << y
						<< " with base speed " << speed << " steps/sec and " << div << " step divisor"
						<< std::endl;
					ctrl->move(point, speed, div, false);
				}
			}
		}  else if (com.compare("rjump") == 0) {
			if (args.size() < 5) {
				std::cout << "Provide arguments" << std::endl;
			} else {
				CoordPlaneStack *ctrl = sysman->getCoord(std::stoi(args.at(0)));
				int x = std::stoi(args.at(1));
				int y = std::stoi(args.at(2));
				float speed = std::stod(args.at(3));
				int div = std::stod(args.at(4));
				if (ctrl == nullptr) {
					std::cout << "Wrong coord id" << std::endl;
				} else {
					motor_point_t point = {x, y};
					std::cout << "\tStarted relative coord # jump " << args.at(0) << " to " << x << "x" << y
						<< " with base speed " << speed << " steps/sec and " << div << " step divisor"
						<< std::endl;
					ctrl->relativeMove(point, speed, div, false);
				}
			}
		} else if (com.compare("arc") == 0) {
			if (args.size() < 8) {
				std::cout << "Provide arguments" << std::endl;
			} else {
				CoordPlaneStack *ctrl = sysman->getCoord(std::stoi(args.at(0)));
				int x = std::stoi(args.at(1));
				int y = std::stoi(args.at(2));
				int cx = std::stoi(args.at(3));
				int cy = std::stoi(args.at(4));
				int sp = std::stoi(args.at(5));
				float speed = std::stod(args.at(6));
				int div = std::stod(args.at(7));
				if (ctrl == nullptr) {
					std::cout << "Wrong coord id" << std::endl;
				} else {
					motor_point_t point = {x, y};
					motor_point_t center = {cx, cy};
					std::cout << "\tStarted coord arc #" << args.at(0) << " to " << x << "x" << y
						<< " with base speed " << speed << " steps/sec and " << div << " step divisor"
						<< std::endl;
					if (ctrl->arc(point, center, sp, speed, div, true) != ErrorCode::NoError) {
						std::cout << "Arc error" << std::endl;
					}
				}
			}
		} else if (com.compare("carc") == 0) {
			if (args.size() < 8) {
				std::cout << "Provide arguments" << std::endl;
			} else {
				CoordPlaneStack *ctrl = sysman->getCoord(std::stoi(args.at(0)));
				int x = std::stoi(args.at(1));
				int y = std::stoi(args.at(2));
				int cx = std::stoi(args.at(3));
				int cy = std::stoi(args.at(4));
				int sp = std::stoi(args.at(5));
				float speed = std::stod(args.at(6));
				int div = std::stod(args.at(7));
				if (ctrl == nullptr) {
					std::cout << "Wrong coord id" << std::endl;
				} else {
					motor_point_t point = {x, y};
					motor_point_t center = {cx, cy};
					std::cout << "\tStarted coord arc #" << args.at(0) << " to " << x << "x" << y
						<< " with base speed " << speed << " steps/sec and " << div << " step divisor"
						<< std::endl;
					if (ctrl->arc(point, center, sp, speed, div, false) != ErrorCode::NoError) {
						std::cout << "Arc error" << std::endl;
					}
				}
			}
		} else if (com.compare("rarc") == 0) {
			if (args.size() < 8) {
				std::cout << "Provide arguments" << std::endl;
			} else {
				CoordPlaneStack *ctrl = sysman->getCoord(std::stoi(args.at(0)));
				int x = std::stoi(args.at(1));
				int y = std::stoi(args.at(2));
				int cx = std::stoi(args.at(3));
				int cy = std::stoi(args.at(4));
				int sp = std::stoi(args.at(5));
				float speed = std::stod(args.at(6));
				int div = std::stod(args.at(7));
				if (ctrl == nullptr) {
					std::cout << "Wrong coord id" << std::endl;
				} else {
					motor_point_t point = {x, y};
					motor_point_t center = {cx, cy};
					std::cout << "\tStarted relative coord arc #" << args.at(0) << " to " << x << "x" << y
						<< " with base speed " << speed << " steps/sec and " << div << " step divisor"
						<< std::endl;
					if (ctrl->relativeArc(point, center, sp, speed, div, true) != ErrorCode::NoError) {
						std::cout << "Arc error" << std::endl;
					}
				}
			}
		} else if (com.compare("rcarc") == 0) {
			if (args.size() < 8) {
				std::cout << "Provide arguments" << std::endl;
			} else {
				CoordPlaneStack *ctrl = sysman->getCoord(std::stoi(args.at(0)));
				int x = std::stoi(args.at(1));
				int y = std::stoi(args.at(2));
				int cx = std::stoi(args.at(3));
				int cy = std::stoi(args.at(4));
				int sp = std::stoi(args.at(5));
				float speed = std::stod(args.at(6));
				int div = std::stod(args.at(7));
				if (ctrl == nullptr) {
					std::cout << "Wrong coord id" << std::endl;
				} else {
					motor_point_t point = {x, y};
					motor_point_t center = {cx, cy};
					std::cout << "\tStarted relative coord arc #" << args.at(0) << " to " << x << "x" << y
						<< " with base speed " << speed << " steps/sec and " << div << " step divisor"
						<< std::endl;
					if (ctrl->relativeArc(point, center, sp, speed, div, false) != ErrorCode::NoError) {
						std::cout << "Arc error" << std::endl;
					}
				}
			}
		} else if (com.compare("cal") == 0) {
			if (args.size() < 2) {
				std::cout << "Provide arguments" << std::endl;
				return;
			}
			int coordNum = std::stoi(args.at(0));
			int coordTrailer = std::stoi(args.at(1));
			CoordPlaneStack *coordController = sysman->getCoord(coordNum);
			if (coordController == nullptr) {
				std::cout << "Wrong coord id" << std::endl;
				return;
			}
			if (coordTrailer != 1 && coordTrailer != 2) {
				std::cout << "Wrong trailer id" << std::endl;
				return;
			}
			coordController->calibrate(coordTrailer == 1 ? TrailerId::Trailer1 : TrailerId::Trailer2);
		} else if (com.compare("graph") == 0) {
			if (args.size() < 13) {
				std::cout << "Provide args" << std::endl;
				return;
			}
			CoordPlane *plane = sysman->getCoord(std::stoi(args.at(0)));
			if (plane == nullptr) {
				std::cout << "Wrong coord id" << std::endl;
				return;
			}
			std::stringstream ss(args.at(1));
			FunctionLexer lexer(ss);
			FunctionParser parser(&lexer);
			Node *node = parser.parse();
			motor_point_t toffset = {std::stoi(args.at(2)), std::stoi(args.at(3))};
			motor_size_t tsize = {std::stoi(args.at(4)), std::stoi(args.at(5))};
			long double minx = std::stold(args.at(6));
			long double maxx = std::stold(args.at(7));
			long double miny = std::stold(args.at(8));
			long double maxy = std::stold(args.at(9));
			long double step = std::stold(args.at(10));
			float speed = std::stod(args.at(11));
			LinearCoordTranslator trans(toffset, tsize);
			coord_point_t min = {minx, miny};
			coord_point_t max = {maxx, maxy};
			GraphBuilder graph(node, min, max, step);
			ErrorCode errcode = graph.build(sysman, plane, &trans, speed);
			if (errcode != ErrorCode::NoError) {
				std::cout << "Graph build error(" << errcode << ")" << std::endl;
			}
		} else {
			std::cout << "Wrong command '" << com << "'" << std::endl;
		}
	}
	
	void RefreshCommand::execute(CLI *cli, std::vector<std::string> &args) {
		sysman->getDeviceManager()->refresh();
	}

	void TaskCommand::execute(CLI *cli, std::vector<std::string> &args) {
		if (args.empty()) {
			std::cout << "Provide parameter" << std::endl;
			return;
		}
		if (args.at(0).compare("new") == 0) {
			CoordTask *task = sysman->createProgrammedTask();
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
			CoordTask *tsk = sysman->getTask(std::stoi(args.at(0)));
			args.erase(args.begin());
			std::string com = args.at(0);
			args.erase(args.begin());
			if (tsk == nullptr) {
				std::cout << "Wrong task id" << std::endl;
				return;
			}
			if (tsk->getType() != CoordTaskType::ProgrammedTask) {
					std::cout << "Can't add steps to this task" << std::endl;
					return;
			}
			ProgrammedCoordTask *task = (ProgrammedCoordTask*) tsk;
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
			} else if (com.compare("rmove") == 0) {
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
				task->addStep(new MoveTaskStep(pnt, sp, true));
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
			} else if (com.compare("rjump") == 0) {
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
				task->addStep(new JumpTaskStep(pnt, sp, true));
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
			} else if (com.compare("rarc") == 0) {
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
				task->addStep(new ArcTaskStep(pnt, cpnt, sp, speed, true));
			} else if (com.compare("rcarc") == 0) {
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
				ArcTaskStep *st = new ArcTaskStep(pnt, cpnt, sp, speed, true);
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
				task->addStep(new CalibrateTaskStep(side == 1 ? TrailerId::Trailer1 : TrailerId::Trailer2));
			} else {
				std::cout << "Wrong command '" << com << "'" << std::endl;
			}
		} else if (args.at(0).compare("exec") == 0) {
			if (args.size() < 4) {
				std::cout << "Wrong argument count" << std::endl;
				return;
			}
			CoordTask *task = sysman->getTask(std::stoi(args.at(1)));
			CoordPlaneStack *coord = sysman->getCoord(std::stoi(args.at(2)));
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
			task->perform(coord, prms, sysman);
		} else if (args.at(0).compare("load") == 0) {
			if (args.size() < 6) {
				std::cout << "Wrong argument count" << std::endl;
				return;
			}
			motor_point_t center = {
				std::stoi(args.at(1)), std::stoi(args.at(2))
			};
			motor_size_t scale = {
				std::stoi(args.at(3)), std::stoi(args.at(4))
			};
			LinearCoordTranslator trans(center, scale);
			std::string path = args.at(5);
			std::ifstream is(path, std::ifstream::in);
			GCodeLexer lex(&is);
			GCodeParser parser(&lex);
			gcode_translate(&parser, &trans, this->sysman);
			is.close();
		} else if (args.at(0).compare("graph") == 0) {
			if (args.size() < 12) {
				std::cout << "Provide args" << std::endl;
				return;
			}
			std::stringstream ss(args.at(1));
			FunctionLexer lexer(ss);
			FunctionParser parser(&lexer);
			Node *node = parser.parse();
			motor_point_t toffset = {std::stoi(args.at(2)), std::stoi(args.at(3))};
			motor_size_t tsize = {std::stoi(args.at(4)), std::stoi(args.at(5))};
			long double minx = std::stold(args.at(6));
			long double maxx = std::stold(args.at(7));
			long double miny = std::stold(args.at(8));
			long double maxy = std::stold(args.at(9));
			long double step = std::stold(args.at(10));
			float speed = std::stod(args.at(11));
			CoordTranslator *trans = new LinearCoordTranslator(toffset, tsize);
			coord_point_t min = {minx, miny};
			coord_point_t max = {maxx, maxy};
			GraphBuilder *graph = new GraphBuilder(node, min, max, step);
			GraphCoordTask *task = new GraphCoordTask(graph, trans, speed);
			std::cout << "New graph task #" << sysman->addTask(task) << std::endl;
		} else {
			std::cout << "Wrong command '" << args.at(0) << "'" << std::endl;
		}
	}
}
