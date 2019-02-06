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

#include "calx/ctrl-lib/logger/TimedJournal.h"
#include "calx/ctrl-lib/logger/Session.h"
#include "calx/ctrl-lib/logger/Filter.h"

namespace CalX {

	TimedJournal::TimedJournal(
	    std::function<std::string(std::chrono::system_clock::time_point)> nameGen,
	    LoggingSeverity severity)
	    : session(nullptr),
	      sessionNameGenerator(nameGen),
	      defaultSeverity(severity) {}

	JournalLogger &TimedJournal::getSession() {
		if (this->session == nullptr) {
			this->session = std::make_unique<DefaultJournalSession>();
			this->session->getController().setFilter(
			    LoggerFilter::severity_at_least(this->defaultSeverity));

			auto now = std::chrono::system_clock::now();
			this->session->getController().newFileSink(
			    "", this->sessionNameGenerator(now));
		}
		return this->session->getLogger();
	}

	void TimedJournal::closeSession() {
		this->session = nullptr;
	}

	JournalLoggerController &TimedJournal::getSessionController() {
		this->getSession();
		return this->session->getController();
	}
}  // namespace CalX