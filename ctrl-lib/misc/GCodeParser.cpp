#include "GCodeParser.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "ctrl-lib/task/CoordTask.h"

namespace _8SMC1 {

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

	GCodeParser::GCodeParser(std::istream *is) {
		this->is = is;
	}	

	GCodeParser::~GCodeParser() {
	}

	// Get next character skipping comments
	int GCodeParser::readChar() {
		int chr = this->is->get();
		while (chr == '(') {
			while (chr != ')' && chr != EOF) {
				chr = this->is->get();
			}
			if (chr == ')') {
				chr = this->is->get();
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
	
	long double parse_double(std::string str) {
		long double out = 0;
		long double out2 = 0;
		for (size_t i = 0; i < str.length() && str.at(i) != '.'; i++) {
			out *= 10;
			out += str.at(i) - '0';
		}
		for (size_t i = str.length() - 1; i < str.length() && str.at(i) != '.'; i--) {
			out2 += str.at(i) - '0';
			out2 /= 10;
		}
		return out + out2;
	}
	
	decimal_number process_number(std::string str) {
		decimal_number out = {0, 0, 0};
		if (str.empty()) {
			return out;
		}
		int8_t c = 0;
		if (str.at(0) == '-') {
			c = 1;
		}
		uint32_t k = 0;
		uint32_t j = 0;
		
		size_t i = c;
		for (;i < str.length() && str.at(i) != '.'; i++) {
			k *= 10;
			k += str.at(i) - '0';
		}
		if (i < str.length() && str.at(i) == '.') {
			i++;
			for (i = str.length() - 1; i < str.length() && str.at(i) != '.'; i--) {
				j *= 10;
				j += str.at(i) - '0';
			}
		}
		out.i = k;
		out.j = j;
		out.s = c;
		return out;
	}
	
	CoordTask *gcode_translate(CoordTranslator &ctr, GCodeParser &parser, SystemManager *sysman) {
		ProgrammedCoordTask *task = sysman->createProgrammedTask();
		GCodeCommand *cmd = nullptr;
		const int CHORD_COUNT = 100;
		while ((cmd = parser.nextCommand()) != nullptr) {
			std::string code = cmd->getCommand();
			if (cmd->hasArg('X') && cmd->hasArg('X')) {
				decimal_number x = process_number(cmd->getArg('X'));
				decimal_number y = process_number(cmd->getArg('Y'));
				motor_point_t real = ctr.get(x, y);
				if (code.compare("G00") == 0) {
					task->addStep(new JumpTaskStep(real, 1.0f));
				} else if (code.compare("G01") == 0) {
					task->addStep(new MoveTaskStep(real, 1.0f));
				} else if (code.compare("G02") == 0 && cmd->hasArg('I') && cmd->hasArg('J')) {
					decimal_number i = process_number(cmd->getArg('I'));
					decimal_number j = process_number(cmd->getArg('J'));
					motor_point_t center = ctr.get(i, j);
					RelArcTaskStep *step = new RelArcTaskStep(real, center, CHORD_COUNT, 1.0f);
					task->addStep(step);
				} else if (code.compare("G03") == 0) {
					decimal_number i = process_number(cmd->getArg('I'));
					decimal_number j = process_number(cmd->getArg('J'));
					motor_point_t center = ctr.get(i, j);
					RelArcTaskStep *step = new RelArcTaskStep(real, center, CHORD_COUNT, 1.0f);
					step->setClockwise(false);
					task->addStep(step);
				}
			}
			delete cmd;
		}
		return task;
	}
}


