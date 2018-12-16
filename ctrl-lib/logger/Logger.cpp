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

#include "ctrl-lib/logger/Logger.h"
#include "ctrl-lib/logger/Session.h"
#include "ctrl-lib/logger/Filter.h"

namespace CalX {

	class DefaultJournalSession : public JournalDefaultLogger,
	                              public JournalSession {
	 public:
		using JournalDefaultLogger::JournalDefaultLogger;

		JournalLogger &getLogger() override {
			return *this;
		}

		JournalLoggerController &getController() override {
			return *this;
		}
	};

	DefaultJournal::DefaultJournal(LoggingSeverity severity)
	    : session(nullptr), defaultSeverity(severity) {}

	JournalLogger &DefaultJournal::getSession() {
		if (this->session == nullptr) {
			this->session = std::make_unique<DefaultJournalSession>();
			this->session->getController().setFilter(
			    LoggerFilter::severity_at_least(this->defaultSeverity));
		}
		return this->session->getLogger();
	}

	void DefaultJournal::closeSession() {
		this->session = nullptr;
	}

	JournalLoggerController &DefaultJournal::getSessionController() {
		this->getSession();
		return this->session->getController();
	}
}  // namespace CalX