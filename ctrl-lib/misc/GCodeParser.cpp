#include "GCodeParser.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "ctrl-lib/task/CoordTask.h"

namespace _8SMC1 {

	GCodeCommand::GCodeCommand(char letter, int com) {
		this->letter = letter;
		this->command = com;
	}

	GCodeCommand::~GCodeCommand() {

	}

	char GCodeCommand::getLetter() {
		return this->letter;
	}
	
	int GCodeCommand::getCommand() {
		return this->command;
	}

	bool GCodeCommand::hasArg(char a) {
		return this->args.count(a) != 0;
	}

	GCodeValue GCodeCommand::getArg(char a) {
		if (this->args.count(a) == 0) {
			GCodeValue val;
			return val;
		}
		return this->args[a];
	}

	void GCodeCommand::setArg(char a, GCodeValue value) {
		this->args[a] = value;
	}

	
	std::map<char, GCodeValue>::iterator GCodeCommand::argsBegin() {
		return this->args.begin();
	}
	std::map<char, GCodeValue>::iterator GCodeCommand::argsEnd() {
		return this->args.end();
	}
	
	GCodeLexer::GCodeLexer(std::istream *is) {
		this->is = is;
	}
	
	GCodeLexer::~GCodeLexer() {
		
	}
	
	int GCodeLexer::readChar() {
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
	
	GCodeField *GCodeLexer::nextField() {
		int chr = readChar();
		while (isspace(chr) && chr != EOF) {
			chr = readChar();
		}
		if (chr == EOF) {
			return nullptr;
		}
		char letter = chr;
		bool fract = false;
		chr = readChar();
		while (isspace(chr) && chr != EOF) {
			chr = readChar();
		}
		if (chr == EOF) {
			return nullptr;
		}
		const size_t BUF_SZ = 256;
		char buf[BUF_SZ];
		size_t buf_offset = 0;
		for (; buf_offset + 1 < BUF_SZ &&
				(isdigit(chr) || chr == '.' || chr == '-');
				chr = readChar()) {
			buf[buf_offset++] = chr;
			if (chr == '.') {
				fract = true;
			}
		}
		is->unget();
		buf[buf_offset] = '\0';
		GCodeField *fld = new GCodeField();
		fld->letter = letter;
		fld->value.fract = fract;
		if (buf_offset != 0) {
			if (fract) {
				fld->value.value.fraction = std::stold(buf);
			} else {
				fld->value.value.integer = std::stoi(buf);
			}
		}
		return fld;
	}
	
	GCodeParser::GCodeParser(GCodeLexer *lexer) {
		this->lexer = lexer;
		this->queue[0] = lexer->nextField();
		this->queue[1] = lexer->nextField();
		this->queue[2] = lexer->nextField();
		this->queue[3] = lexer->nextField();
		
		this->PARAMETERS.insert('X');
		this->PARAMETERS.insert('Y');
		this->PARAMETERS.insert('Z');
		this->PARAMETERS.insert('I');
		this->PARAMETERS.insert('J');
		this->PARAMETERS.insert('S');
		this->PARAMETERS.insert('P');
		this->PARAMETERS.insert('D');
		this->PARAMETERS.insert('H');
		this->PARAMETERS.insert('R');
		this->PARAMETERS.insert('Q');
	}
	
	GCodeParser::~GCodeParser() {
		for (size_t i = 0; i < 4; i++) {
			if (this->queue[i] != nullptr) {
				delete this->queue[i];
			}
		}
	}
	
	void GCodeParser::nextField() {
		if (this->queue[0] != nullptr) {
			delete this->queue[0];
		}
		this->queue[0] = this->queue[1];
		this->queue[1] = this->queue[2];
		this->queue[2] = this->queue[3];
		this->queue[3] = this->lexer->nextField();
	}
	
	GCodeCommand *GCodeParser::nextCommand() {
		GCodeCommand *com = nullptr;
		if (this->queue[0] == nullptr) {
			return nullptr;
		}
		com = new GCodeCommand(this->queue[0]->letter, this->queue[0]->value.intValue());
		this->nextField();
		while (this->queue[0] != nullptr &&
			this->PARAMETERS.count(this->queue[0]->letter) > 0) {
			com->setArg(this->queue[0]->letter, this->queue[0]->value);
			this->nextField();
		}
		return com;
	}
	
	CoordTask *gcode_translate(GCodeParser *parser, CoordTranslator *translator, SystemManager *sysman) {
		ProgrammedCoordTask *task = sysman->createProgrammedTask();
		coord_point_t last = {0, 0};
		GCodeCommand *cmd = nullptr;
		const int CHORD_COUNT = 100;
		while ((cmd = parser->nextCommand()) != nullptr) {
			
			if (cmd->getLetter() == 'G') {
				long double x = cmd->hasArg('X') ? cmd->getArg('X').fractValue() : last.x;
				long double y = cmd->hasArg('Y') ? cmd->getArg('Y').fractValue() : last.y;
				motor_point_t real = translator->get(x, y);
				switch (cmd->getCommand()) {
					case 0:
						task->addStep(new JumpTaskStep(real, 1.0f));
					break;
					case 1:
						task->addStep(new MoveTaskStep(real, 1.0f));
					break;
					case 2: {
						long double i = cmd->getArg('I').fractValue();
						long double j = cmd->getArg('J').fractValue();
						motor_point_t center = translator->get(i, j);
						RelArcTaskStep *step = new RelArcTaskStep(real, center, CHORD_COUNT, 1.0f);
						step->setClockwise(true);
						task->addStep(step);
					} break;
					case 3: {
						long double i = cmd->getArg('I').fractValue();
						long double j = cmd->getArg('J').fractValue();
						motor_point_t center = translator->get(i, j);
						RelArcTaskStep *step = new RelArcTaskStep(real, center, CHORD_COUNT, 1.0f);
						step->setClockwise(false);
						task->addStep(step);
					} break;
				}
				last = {x, y};
			}
			
			delete cmd;
		}
		return task;
	}
}


