#include "GCodeParser.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace Controller {

	GCodeCommand::GCodeCommand(std::string com) {
		this->command = com;
	}

	GCodeCommand::~GCodeCommand() {

	}

	std::string GCodeCommand::getCommand() {
		return this->command;
	}

	bool GCodeCommand::hasArg(char a) {
		return this->args.count(a) != 0;
	}

	std::string GCodeCommand::getArg(char a) {
		if (this->args.count(a) == 0) {
			return "";
		}
		return this->args[a];
	}

	void GCodeCommand::setArg(char a, std::string value) {
		this->args[a] = value;
	}

	
	std::map<char, std::string>::iterator GCodeCommand::argsBegin() {
		return this->args.begin();
	}
	std::map<char, std::string>::iterator GCodeCommand::argsEnd() {
		return this->args.end();
	}

	GCodeParser::GCodeParser(FILE *fp) {
		this->fp = fp;
	}	

	GCodeParser::~GCodeParser() {
		fclose(this->fp);
	}

	// Get next character skipping comments
	int GCodeParser::readChar() {
		int chr = fgetc(this->fp);
		while (chr == '(') {
			while (chr != ')' && chr != EOF) {
				chr = fgetc(this->fp);
			}
			if (chr == ')') {
				chr = fgetc(this->fp);
			}
		}
		return chr;
	}

	GCodeCommand *GCodeParser::nextCommand() {
		std::vector<std::string> arr;
		const int WORD_LENGTH = 256;
		char word[WORD_LENGTH];
		int word_offset = 0;
		int chr; // Input character
		// Skip starting spaces
		while (isspace(chr = readChar())) {}
		if (chr == EOF) { // Skip EOF
			return nullptr;
		}
		
		while (chr != '\n' && chr != EOF) {
			if (chr == ' ') {
				word[word_offset] = '\0';
				arr.push_back(std::string(word));
				word_offset = 0;
			} else {
				word[word_offset++] = chr;
			}
			chr = readChar();
		}
		if (word_offset > 0) {
			word[word_offset] = '\0';
			arr.push_back(std::string(word));
		}

		if (arr.empty()) {
			return nullptr;
		}

		GCodeCommand *com = new GCodeCommand(arr[0]);
		for (unsigned int i = 1; i < arr.size(); i++) {
			std::string arg = arr[i];
			char a = arg.c_str()[0];
			arg.erase(arg.begin());
			com->setArg(a, arg);
		}
		return com;
	}
}

/* Currently this parser is not used by project, however it will be
   included in it later. For testing purposes I created simple
   main function, that reads file passes in first argument and 
   prints parsed commands. */

using namespace Controller;

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cout << "Provide file name" << std::endl;
		return EXIT_FAILURE;
	}
	GCodeParser gp(fopen(argv[1], "r"));

	GCodeCommand *com;
	while ((com = gp.nextCommand()) != nullptr) {
		std::cout << "Command: " << com->getCommand() << std::endl;
		for (auto it = com->argsBegin(); it != com->argsEnd(); ++it) {
			std::cout << "\tArgument: " << it->first << " = " << it->second << std::endl;
		}
		delete com;
	}
	return EXIT_SUCCESS;
}
