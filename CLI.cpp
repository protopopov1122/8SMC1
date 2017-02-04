#include "CLI.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

namespace Controller {
	
	CLI::CLI() {

	}

	CLI::~CLI() {
		for (const auto& kv : this->commands) {
			delete kv.second;
		}
	}

	void CLI::error(std::string err) {
		std::cout << "Error: " << err << std::endl;
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
		std::cout << PROMPT;
		std::string input;
		getline(std::cin, input);

		// TODO Trim line
		if (input.length() == 0) {
			return true;
		}
		// Split line into command and arguments
		std::string delimiter = " ";
		std::vector<std::string> args;
		size_t pos = 0;
		std::string token;
		while ((pos = input.find(delimiter)) != std::string::npos) {
			token = input.substr(0, pos);
			args.push_back(token);
			input.erase(0, pos + delimiter.length());
		}
		args.push_back(input);
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
		cmd->execute(args);

		
		return true;
	}
}
