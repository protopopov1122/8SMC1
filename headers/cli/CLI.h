/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License as published
   by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CALX_CLI_H_
#define CALX_CLI_H_

#include <cinttypes>
#include <iosfwd>
#include <map>
#include <string>
#include <vector>

/* CLI is abstract command line implementation that store
   command definitions and work with command line:
   read command, split it in arguments and operation,
   call operations.
   Ceratin command implementations see in DevCLI.*/

namespace CalX {

	class CLI;  // Forward referencing

	class CLICommand {
	 public:
		virtual ~CLICommand(){};
		virtual void execute(CLI *, std::vector<std::string> &) = 0;
	};

	class CLI {
	 public:
		CLI(std::ostream &, std::istream &);
		virtual ~CLI();
		bool shell();
		void error(const std::string &);
		void addCommand(const std::string &, CLICommand *);

	 private:
		std::map<std::string, CLICommand *> commands;
		std::ostream *out;
		std::istream *in;
	};
}  // namespace CalX

#endif
