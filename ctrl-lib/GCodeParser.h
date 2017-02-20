#ifndef _8SMC1_GCODE_PARSER_H_
#define _8SMC1_GCODE_PARSER_H_

#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "SystemManager.h"
#include "CoordTask.h"
#include "CoordTranslator.h"

/* This file contains simple G-Code parser. It allows parsing simple G-Code files
   line-by-line skipping black lines and comments. Each parsed line is
   allocated as GCodeCommand object. */

namespace _8SMC1 {
	
	
	class GCodeCommand {
		public:
			GCodeCommand(std::string);
			virtual ~GCodeCommand();
			std::string getCommand();
			bool hasArg(char);
			std::string getArg(char);
			void setArg(char, std::string);
			std::map<char, std::string>::iterator argsBegin();
			std::map<char, std::string>::iterator argsEnd();
		private:
			std::string command;
			std::map<char, std::string> args;
	};
	
	class GCodeParser {
		public:
			GCodeParser(std::istream*);
			virtual ~GCodeParser();
			GCodeCommand *nextCommand();
		private:
			int readChar();
			std::istream *is;		
	};
	
	CoordTask *gcode_translate(CoordTranslator&, GCodeParser&, SystemManager*);
}

#endif
