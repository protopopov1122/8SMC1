#ifndef _8SMC1_CLI_H_
#define _8SMC1_CLI_H_

#include <string>
#include <cinttypes>
#include <vector>
#include <map>
#include <iostream>

namespace _8SMC1 {
		
	class CLI; // Forward referencing
		
	class CLICommand {
		public:
			virtual ~CLICommand() {};
			virtual void execute(CLI*, std::vector<std::string>&) = 0;
		protected:
			std::string requireArgument(std::vector<std::string>&, size_t, std::string, std::string);
			int requireArgument(std::vector<std::string>&, size_t, std::string,
						int64_t);
			float requireArgument(std::vector<std::string>&, size_t, std::string,
						float);
			
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
