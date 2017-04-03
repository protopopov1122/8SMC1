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


#ifndef CALX_GCODE_PARSER_H_
#define CALX_GCODE_PARSER_H_

#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include "ctrl-lib/SystemManager.h"
#include "ctrl-lib/ConfigManager.h"
#include "ctrl-lib/task/CoordTask.h"
#include "ctrl-lib/translator/CoordTranslator.h"

/* GCode parser can parse unforammted gcode files.
   Each gcode statement is represented as command object.
   Parser and lexer are separated and use STL streams.
   GCode is splitted in fields by lexer. Each
   field contains command letter and parameter
   (fractional or integer value). Parser can
   recognise some command letters and join them
   with next fields. */

namespace CalX {
		
	enum GCodeOpcode {
		GCode_Jump = 0, GCode_Move = 1,
		GCode_Clockwise_Arc = 2, GCode_CounterClockwise_Arc = 3
	};	
		
	struct GCodeValue {
		bool fract;
		union {
			double fraction;
			int64_t integer;
		} value;
		
		int64_t intValue() {return fract ? (int64_t) value.fraction : value.integer;}
		double fractValue() {return fract ? value.fraction : (double) value.integer;}
	};
	
	class GCodeCommand {
		public:
			GCodeCommand(char, int);
			virtual ~GCodeCommand();
			char getLetter();
			int getCommand();
			bool hasArg(char);
			GCodeValue getArg(char);
			void setArg(char, GCodeValue);
			std::map<char, GCodeValue>::iterator argsBegin();
			std::map<char, GCodeValue>::iterator argsEnd();
		private:
			char letter;
			int command;
			std::map<char, GCodeValue> args;
	};
	
	struct GCodeField {
		char letter;
		GCodeValue value;
	};
	
	class GCodeLexer {
		public:
			GCodeLexer(std::istream*);
			virtual ~GCodeLexer();
			GCodeField *nextField();
		private:
			int readChar();
			std::istream *is;
	};
	
	class GCodeParser {
		public:
			GCodeParser(GCodeLexer*);
			virtual ~GCodeParser();
			GCodeCommand *nextCommand();
		private:
			void nextField();
			GCodeLexer *lexer;
			GCodeField *queue[4];
			std::set<char> PARAMETERS;
	};
	
	CoordTask *gcode_translate(GCodeParser*, CoordTranslator*, ProgrammedCoordTask*, ConfigManager*);
}

#endif
