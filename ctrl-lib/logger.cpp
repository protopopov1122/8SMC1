/*
        Copyright (c) 2017-2018 Jevgenijs Protopopovs

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

#include "ctrl-lib/logger.h"
#include "ctrl-lib/logger/Session.h"

using namespace CalX;

JournalDefaultSession globalJournal(LoggingSeverity::Debug);

void LOGGER_LOG(const std::string &sinkName, const std::string &tag, const std::string &message, const char *file, int line) {
	const JournalSink &sink = globalJournal.getSink(sinkName);
	try {
		sink.log(LoggingSeverity::Debug, message, tag, SourcePosition(file, line));
	} catch (...) {
	}
}

void SET_LOGGER(const std::string &sinkName, std::ostream *stream) {
	JournalStreamSinkFactory factory(*stream);
	globalJournal.newSink(sinkName, factory, sinkName.compare(INFORMATION) == 0);
}
