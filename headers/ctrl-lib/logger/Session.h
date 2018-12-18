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

#ifndef CALX_CTRL_LIB_LOGGING_SESSION_H_
#define CALX_CTRL_LIB_LOGGING_SESSION_H_

#include "ctrl-lib/logger/Journal.h"
#include "ctrl-lib/logger/Sink.h"
#include <map>

namespace CalX {

	class JournalDefaultLogger : public JournalLoggerController,
	                             public JournalLogger {
	 public:
		JournalDefaultLogger();

		const JournalSink *getSink(const std::string &) const override;
		void getSinks(std::vector<std::reference_wrapper<const JournalSink>> &)
		    const override;
		void log(const LogEntry &) const override;

		void setFilter(std::function<bool(const LogEntry &)>) override;
		JournalSink &newStreamSink(const std::string &, std::ostream &,
		                           bool) override;
		JournalSink &newFileSink(const std::string &, const std::string &,
		                         bool) override;
		void dropSink(const std::string &) override;

	 private:
		std::map<std::string, std::shared_ptr<JournalSink>> sinks;
		std::function<bool(const LogEntry &)> filter;
	};

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
}  // namespace CalX

#endif