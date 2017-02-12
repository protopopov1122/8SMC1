#ifndef _8SMC1_CLI_H_
#define _8SMC1_CLI_H_

#include <string>
#include <vector>
#include <map>

namespace _8SMC1 {
		
	class CLICommand {
		public:
			virtual ~CLICommand() {};
			virtual void execute(std::vector<std::string>&) = 0;
	};
	
	class CLI {
		public:
			CLI();
			virtual ~CLI();
			bool shell();
			void error(std::string);
			void addCommand(std::string, CLICommand*);
		private:
			std::map<std::string, CLICommand*> commands;
	};
}

#endif
