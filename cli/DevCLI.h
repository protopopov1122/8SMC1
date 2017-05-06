/*
    Copyright (c) 2017 Jevgenijs Protopopovs

    This file is part of CalX project.

    CalX is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CalX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/


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
	class CLISystemCommand : public CLICommand {
		public:
			CLISystemCommand(SystemManager *sysman) {
				this->sysman = sysman;
			}
			virtual ~CLISystemCommand() {}
			virtual void execute(CLI *cli, std::vector<std::string> &args) = 0;
		protected:
			SystemManager *sysman;
	};
	
	#define CMD(name) class name : public CLISystemCommand {\
		public:\
			name(SystemManager *sysman) :\
				CLISystemCommand::CLISystemCommand(sysman) {};\
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
