/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calx/cli/CLI.h"
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

namespace CalX {

	CLI::CLI(std::ostream &os, std::istream &is) : out(os), in(is) {}

	void CLI::error(const std::string &err) {
		this->out << "Error: " << err << std::endl;
	}

	void CLI::addCommand(const std::string &name,
	                     std::unique_ptr<CLICommand> cmd) {
		if (this->commands.count(name) != 0) {
			this->error("(CLI) Command '" + name + "' already exists");
			return;
		}
		this->commands[name] = std::move(cmd);
	}

	bool CLI::shell() {
		// Read and execute one shell command
		const std::string PROMPT = ">>> ";
		this->out << PROMPT;
		std::string input;
		getline(this->in, input);

		if (input.length() == 0) {
			return true;
		}
		// Split line into command and arguments
		std::vector<std::string> args;
		std::size_t pos = 0;
		bool quote = false;
		const char *line = input.c_str();
		args.push_back("");
		while (pos < strlen(line)) {
			char chr = line[pos++];
			if (chr == '\"') {
				quote = !quote;
				continue;
			}
			if (chr != '\\' && chr == ' ' && !quote &&
			    !args.at(args.size() - 1).empty()) {
				args.push_back("");
			} else {
				if (chr == '\\' && pos < strlen(line)) {
					switch (line[pos++]) {
						case 'n':
							chr = '\n';
							break;
						case 't':
							chr = '\t';
							break;
						case 'r':
							chr = '\r';
							break;
						case '\\':
							chr = '\\';
							break;
						case '\"':
							chr = '\"';
							break;
						case '\'':
							chr = '\'';
							break;
						case ' ':
							chr = ' ';
							break;
					}
				}
				if (quote || chr != ' ') {
					std::string str = args.at(args.size() - 1);
					str += chr;
					args[args.size() - 1] = str;
				}
			}
		}

		// Build command object
		std::string command = args.at(0);
		args.erase(args.begin());

		if (command.length() >= 4 && command.compare(0, 4, "exit") == 0) {
			return false;
		}

		if (this->commands.count(command) == 0) {
			this->error("Unknown command '" + command + "'");
			return true;
		}

		try {
			CLICommand &cmd = *this->commands[command];
			cmd.execute(this, args);
		} catch (...) {
			std::cout << "Command execution error" << std::endl;
		}

		return true;
	}
}  // namespace CalX
