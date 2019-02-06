/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License as published
   by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calx/ctrl-lib/logger/Shortcuts.h"

namespace CalX {

	JournalSinkStream Debug(JournalLogger &logger, const std::string &tag,
	                        const SourcePosition &position) {
		return logger.stream(LoggingSeverity::Debug, tag, position);
	}

	JournalSinkStream Info(JournalLogger &logger, const std::string &tag,
	                       const SourcePosition &position) {
		return logger.stream(LoggingSeverity::Info, tag, position);
	}

	JournalSinkStream Warning(JournalLogger &logger, const std::string &tag,
	                          const SourcePosition &position) {
		return logger.stream(LoggingSeverity::Warning, tag, position);
	}

	JournalSinkStream Error(JournalLogger &logger, const std::string &tag,
	                        const SourcePosition &position) {
		return logger.stream(LoggingSeverity::Error, tag, position);
	}

	JournalSinkStream Critical(JournalLogger &logger, const std::string &tag,
	                           const SourcePosition &position) {
		return logger.stream(LoggingSeverity::Critical, tag, position);
	}
}  // namespace CalX