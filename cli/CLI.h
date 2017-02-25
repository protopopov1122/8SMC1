#ifndef _8SMC1_CLI_H_
#define _8SMC1_CLI_H_

#include <string>
#include <cinttypes>
#include <vector>
#include <map>
#include <iostream>

/* CLI is abstract command line implementation that store
   command definitions and work with command line:
   read command, split it in arguments and operation,
   call operations.
   Ceratin command implementations see in DevCLI.*/

namespace _8SMC1 {
		
	class CLI; // Forward referencing
		
	class CLICommand {
		public:
			virtual ~CLICommand() {};
			virtual void execute(CLI*, std::vector<std::string>&) = 0;
			
	};
	
	class CLI {
		public:
			CLI(std::ostream&, std::istream&);
			virtual ~CLI();
			bool shell();
			void error(std::string);
			void addCommand(std::string, CLICommand*);
		private:
			std::map<std::string, CLICommand*> commands;
			std::ostream *out;
			std::istream *in;
	};
}

#endif
