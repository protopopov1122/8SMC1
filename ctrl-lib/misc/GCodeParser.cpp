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


#include "GCodeParser.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "ctrl-lib/task/CoordTask.h"

namespace CalX {

    GCodeCommand::GCodeCommand(char letter, int64_t com) {
		this->letter = letter;
		this->command = com;
		INIT_LOG("GCodeCommand");
	}

	GCodeCommand::~GCodeCommand() {
		DESTROY_LOG("GCodeCommand");
	}

	char GCodeCommand::getLetter() {
		return this->letter;
	}
	
    int64_t GCodeCommand::getCommand() {
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
		INIT_LOG("GCodeLexer");
	}
	
	GCodeLexer::~GCodeLexer() {
		DESTROY_LOG("GCodeLexer");
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
        char letter = (char) chr;
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
            buf[buf_offset++] = (char) chr;
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
                fld->value.value.fraction = std::stod(buf);
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

		INIT_LOG("GCodeParser");
	}
	
	GCodeParser::~GCodeParser() {
		for (size_t i = 0; i < 4; i++) {
			if (this->queue[i] != nullptr) {
				delete this->queue[i];
			}
		}
		DESTROY_LOG("GCodeParser");
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
	
	CoordTask *gcode_translate(GCodeParser *parser, CoordTranslator *translator, ProgrammedCoordTask *task, ConfigManager *conf) {
		motor_point_t offset = translator->get(0, 0);
		coord_point_t last = {0, 0};
		int invert = 1;
		if (translator->get(1, 0).x < offset.x) {
			invert *= -1;
		}
		if (translator->get(0, 1).y < offset.y) {
			invert *= -1;
		}
		GCodeCommand *cmd = nullptr;
        const int_conf_t CHORD_COUNT = conf->getEntry("core")->getInt("chord_count", 100);
		while ((cmd = parser->nextCommand()) != nullptr) {
			
			if (cmd->getLetter() == 'G') {
				double x = cmd->hasArg('X') ? cmd->getArg('X').fractValue() : last.x;
				double y = cmd->hasArg('Y') ? cmd->getArg('Y').fractValue() : last.y;
				motor_point_t real = translator->get(x, y);
				switch (cmd->getCommand()) {
					case GCodeOpcode::GCode_Jump:
						task->addStep(new JumpTaskStep(real, 1.0f));
					break;
					case GCodeOpcode::GCode_Move:
						task->addStep(new MoveTaskStep(real, 1.0f));
					break;
					case GCodeOpcode::GCode_Clockwise_Arc: {
						double i = cmd->getArg('I').fractValue();
						double j = cmd->getArg('J').fractValue();
						motor_point_t center = translator->get(i, j);
						center.x -= offset.x;
						center.y -= offset.y;
                        RelArcTaskStep *step = new RelArcTaskStep(real, center, (int) CHORD_COUNT, 1.0f);
						step->setClockwise(invert == 1);
						task->addStep(step);
					} break;
					case GCodeOpcode::GCode_CounterClockwise_Arc: {
						double i = cmd->getArg('I').fractValue();
						double j = cmd->getArg('J').fractValue();
						motor_point_t center = translator->get(i, j);
						center.x -= offset.x;
						center.y -= offset.y;
                        RelArcTaskStep *step = new RelArcTaskStep(real, center, (int) CHORD_COUNT, 1.0f);
						step->setClockwise(invert != 1);
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


