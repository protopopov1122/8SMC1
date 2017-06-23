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


#include "ctrl-lib/gcode/GCodeStream.h"
#include <string.h>
#include <regex>

namespace CalX {
	
	GCodeToken::GCodeToken(char operation, GCodeParameter prm) {
		this->operation = operation;
		this->parameter = prm;
	}
	
	char GCodeToken::getOperation() {
		return this->operation;
	}
	
	GCodeParameter GCodeToken::getParameter() {
		return this->parameter;
	}
	
	GCodeCmd::GCodeCmd(GCodeOperation oper) {
		this->operation = oper;
	}
	
	GCodeOperation GCodeCmd::getOperation() {
		return this->operation;
	}
	
	bool GCodeCmd::hasArgument(char arg) {
		return this->args.count(arg) != 0;
	}
	
	void GCodeCmd::putArgument(char arg, GCodeParameter prm) {
		this->args.insert(std::make_pair(arg, prm));
	}
	
	GCodeParameter GCodeCmd::getArgument(char arg) {
		if (this->args.count(arg) == 0) {
			GCodeParameter prm;
			prm.type = GCodeParameterType::Unknown;
			return prm;
		} else {
			return this->args.at(arg);
		}
	}
	
	GCodeStream::GCodeStream(std::istream &is) {
		std::vector<std::string> lexems;
		std::vector<GCodeToken> tokens;
		this->find_lexems(is, lexems);
		this->find_tokens(lexems, tokens);
		this->find_commands(tokens, this->commands);
		this->position = 0;
	}
	
	void GCodeStream::reset() {
		this->position = 0;
	}
	
	bool GCodeStream::hasNext() {
		return this->position < this->commands.size();
	}
	
	GCodeCmd GCodeStream::next() {
		if (this->position >= this->commands.size()) {
			GCodeCmd cmd(GCodeOperation::None);
			return cmd;
		} else {
			return this->commands.at(this->position++);
		}
	}
	
	void GCodeStream::find_lexems(std::istream &is, std::vector<std::string> &lexems) {
		std::string lexem;
		int comment_mode = 0;
		while (!is.eof() && is.good()) {
			int input = is.get();
			if (input == EOF) {
				break;
			} else if (isspace(input)) {
				if (!lexem.empty()) {
					lexems.push_back(lexem);
					lexem.clear();
				}
			} else if (input == '(') {
				comment_mode++;
			} else if (input == ')') {
				comment_mode--;
			} else if (comment_mode == 0) {
				lexem.push_back(input);
			}
		}
		if (!lexem.empty()) {
			lexems.push_back(lexem);
		}
	}
	
	void GCodeStream::find_tokens(std::vector<std::string> &lexems, std::vector<GCodeToken> &tokens) {
		for (std::vector<std::string>::iterator it = lexems.begin(); it != lexems.end(); ++it) {
			std::string lexem = *it;
			char operation = lexem.at(0);
			GCodeParameter prm;
			prm.type = GCodeParameterType::Unknown;
			if (lexem.size() > 1) {
				std::string par = lexem.substr(1, lexem.size() - 1);
				this->parse_parameter(prm, par);
			} else {
				++it;
				if (it != lexems.end()) {
					std::string par = *it;
					if (!this->parse_parameter(prm, par)) {
						--it;
					}
				} else {
					GCodeToken token(operation, prm);
					tokens.push_back(token);
					break;
				}
			}
			GCodeToken token(operation, prm);
			tokens.push_back(token);
		}
	}
	
	void GCodeStream::find_commands(std::vector<GCodeToken> &tokens, std::vector<GCodeCmd> &commands) {
		GCodeOperation oper = GCodeOperation::None;
		for (std::vector<GCodeToken>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
			GCodeToken tk = *it;
			if (tk.getOperation() == 'G' && tk.getParameter().type == GCodeParameterType::Integer) {
				int64_t opr = tk.getParameter().value.integer;
				if ((opr >= 0 && opr <= 3) ||
					opr == 20 ||
					opr == 21 ||
					opr == 28) {
					oper = static_cast<GCodeOperation>(tk.getParameter().value.integer);
					GCodeCmd cmd(oper);
					commands.push_back(cmd);
				}
			} else if (oper != GCodeOperation::None) {
				char parname = tk.getOperation();
				if (parname == 'X' ||
					parname == 'Y' ||
					parname == 'Z' ||
					parname == 'I' ||
					parname == 'J') {
					if (commands.at(commands.size() - 1).hasArgument(parname)) {
						GCodeCmd cmd(oper);
						cmd.putArgument(parname, tk.getParameter());
						commands.push_back(cmd);
					} else {
						commands.at(commands.size() - 1).putArgument(parname, tk.getParameter());
					}
				}
			}
		}
	}
	
	bool GCodeStream::parse_parameter(GCodeParameter &prm, std::string par) {
		static std::regex int_regex("[-+]?[0-9]+");
		static std::regex float_regex("[-+]?[0-9]*\\.?[0-9]+");
		if (std::regex_match(par, int_regex)) {
			prm.type = GCodeParameterType::Integer;
			prm.value.integer = stoll(par);
		} else if (std::regex_match(par, float_regex)) {
			prm.type = GCodeParameterType::Real;
			prm.value.real = stold(par);
		} else {
			prm.type = GCodeParameterType::Unknown;
			return false;
		}
		return true;
	}
}