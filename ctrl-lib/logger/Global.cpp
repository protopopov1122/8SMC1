/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calx/ctrl-lib/logger/Global.h"
#include "calx/ctrl-lib/logger/TimedJournal.h"

namespace CalX {

	static const std::string Errors = "errors";
	static const std::string Warnings = "warnings";
	static const std::string Debug = "debug";
	static const std::string Information = "info";

	DefaultJournal GlobalLogger::logger(LoggingSeverity::Debug);

	JournalLogger &GlobalLogger::getLogger() {
		return GlobalLogger::logger.getSession();
	}

	const std::string &GlobalLogger::getSink(GlobalLoggingSink id) {
		switch (id) {
			case GlobalLoggingSink::Errors:
				return Errors;
			case GlobalLoggingSink::Warnings:
				return Warnings;
			case GlobalLoggingSink::Debug:
				return Debug;
			case GlobalLoggingSink::Information:
				return Information;
			default:
				return Information;
		}
	}

	JournalLoggerController &GlobalLogger::getController() {
		return GlobalLogger::logger.getSessionController();
	}
}  // namespace CalX