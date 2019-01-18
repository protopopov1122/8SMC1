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

#include "ctrl-lib/logger/Logger.h"
#include "ctrl-lib/logger/Session.h"
#include "ctrl-lib/logger/Filter.h"
#include <iostream>

namespace CalX {

	DefaultJournal::DefaultJournal(LoggingSeverity severity)
	    : session(nullptr), defaultSeverity(severity) {}

	JournalLogger &DefaultJournal::getSession() {
		return this->openSession().getLogger();
	}

	void DefaultJournal::closeSession() {
		this->session = nullptr;
	}

	JournalLoggerController &DefaultJournal::getSessionController() {
		return this->openSession().getController();
	}

	JournalSession &DefaultJournal::openSession() {
		if (this->session == nullptr) {
			this->session = std::make_unique<DefaultJournalSession>();
			this->session->getController().setFilter(
			    LoggerFilter::severity_at_least(this->defaultSeverity));
			this->session->getController().newStreamSink("default", std::cout);
		}
		return *this->session;
	}
}  // namespace CalX