#ifndef _8SMC1_GCODE_PARSER_H_
#define _8SMC1_GCODE_PARSER_H_

#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include "ctrl-lib/SystemManager.h"
#include "ctrl-lib/task/CoordTask.h"
#include "ctrl-lib/translator/CoordTranslator.h"

/* This file contains simple G-Code parser. It allows parsing simple G-Code files
   line-by-line skipping black lines and comments. Each parsed line is
   allocated as GCodeCommand object. */

namespace _8SMC1 {
		
	struct GCodeValue {
		bool fract;
		union {
			long double fraction;
			int64_t integer;
		} value;
		
		int64_t intValue() {return fract ? (int64_t) value.fraction : value.integer;}
		long double fractValue() {return fract ? value.fraction : (long double) value.integer;}
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
	
	CoordTask *gcode_translate(GCodeParser*, CoordTranslator*, SystemManager*);
}

#endif
