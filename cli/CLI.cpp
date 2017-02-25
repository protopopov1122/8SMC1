#include "CLI.h"
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <iostream>

namespace _8SMC1 {
	
	CLI::CLI(std::ostream &os, std::istream &is) {
		this->in = &is;
		this->out = &os;
	}

	CLI::~CLI() {
		for (const auto& kv : this->commands) {
			delete kv.second;
		}
	}

	void CLI::error(std::string err) {
		*out << "Error: " << err << std::endl;
	}

	void CLI::addCommand(std::string name, CLICommand *cmd) {
		if (this->commands.count(name) != 0) {
			this->error("(CLI) Command '"+name+"' already exists");
			return;
		}
		this->commands[name] = cmd;
	}

	bool CLI::shell() {
		// Read and execute one shell command
		const std::string PROMPT = ">>> ";
		*out << PROMPT;
		std::string input;
		getline(*in, input);

		if (input.length() == 0) {
			return true;
		}
		// Split line into command and arguments
		std::vector<std::string> args;
		size_t pos = 0;
		bool quote = false;
		const char *line = input.c_str();
		args.push_back("");
		while (pos < strlen(line)) {
			char chr = line[pos++];
			if (chr == '\"') {
				quote = !quote;
				continue;
			}
			if (chr != '\\' && chr == ' ' && !quote && !args.at(args.size() - 1).empty()) {
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
			this->error("Unknown command '"+command+"'");
			return true;
		}

		CLICommand *cmd = this->commands[command];
		cmd->execute(this, args);

		
		return true;
	}
}
