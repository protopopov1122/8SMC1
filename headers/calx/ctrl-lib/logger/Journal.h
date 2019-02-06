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

#ifndef CALX_CTRL_LIB_LOGGER_JOURNAL_H_
#define CALX_CTRL_LIB_LOGGER_JOURNAL_H_

#include "calx/platform.h"
#include "calx/ctrl-lib/logger/Level.h"
#include <string>
#include <type_traits>
#include <vector>
#include <functional>
#include <memory>
#include <sstream>

namespace CalX {

	struct SourcePosition {
		SourcePosition() : file(""), line(0) {}
		SourcePosition(const std::string &file, unsigned int line)
		    : file(file), line(line) {}
		SourcePosition(const char *file, unsigned int line)
		    : file(file), line(line) {}
		SourcePosition(const SourcePosition &position)
		    : file(position.file), line(position.line) {}
		bool isEmpty() const {
			return this->file.size() == 0 || this->line == 0;
		}

		const std::string file;
		const unsigned int line;
	};

	struct LogEntry {
		LogEntry(LoggingSeverity severity, const std::string &message,
		         const std::string &tag = "",
		         const SourcePosition &position = SourcePosition())
		    : severity(severity), message(message), tag(tag), position(position) {}

		const LoggingSeverity severity;
		const std::string message;
		const std::string tag;
		const SourcePosition position;
	};

	class JournalSink;

	struct Flush {};

	class Loggable {
	 public:
		virtual ~Loggable() = default;
		virtual void log(const LogEntry &) const = 0;
	};

	class JournalSinkStream {
	 public:
		JournalSinkStream(const Loggable &, LoggingSeverity,
		                  const std::string & = "",
		                  const SourcePosition & = SourcePosition());
		~JournalSinkStream();

		void flush();

		JournalSinkStream &operator<<(Flush flush) {
			this->flush();
			return *this;
		}

		template<typename T>
		JournalSinkStream &operator<<(T &value) {
			this->buffer << value;
			return *this;
		}

		template<typename T>
		JournalSinkStream &operator<<(T &&value) {
			this->buffer << value;
			return *this;
		}

	 private:
		const Loggable &log;
		LoggingSeverity severity;
		std::string tag;
		SourcePosition position;
		std::stringstream buffer;
	};

	class JournalSink : public Loggable {
	 public:
		virtual ~JournalSink() = default;
		virtual const std::string &getName() const = 0;
		virtual void setFilter(std::function<bool(const LogEntry &)>) = 0;
		virtual void setFormatter(
		    std::function<void(const LogEntry &, std::ostream &)>) = 0;

		JournalSinkStream stream(LoggingSeverity, const std::string & = "",
		                         const SourcePosition & = SourcePosition()) const;
	};

	class JournalSinkFactory {
	 public:
		virtual ~JournalSinkFactory() = default;
		virtual std::unique_ptr<JournalSink> newSink(const std::string &) const = 0;
	};

	class JournalLogger : public Loggable {
	 public:
		virtual ~JournalLogger() = default;
		virtual const JournalSink *getSink(const std::string &) const = 0;
		virtual void getSinks(
		    std::vector<std::reference_wrapper<const JournalSink>> &) const = 0;

		JournalSinkStream stream(LoggingSeverity, const std::string & = "",
		                         const SourcePosition & = SourcePosition()) const;
	};

	class JournalLoggerController {
	 public:
		virtual ~JournalLoggerController() = default;
		virtual void setFilter(std::function<bool(const LogEntry &)>) = 0;
		virtual JournalSink &newStreamSink(const std::string &, std::ostream &) = 0;
		virtual JournalSink &newFileSink(const std::string &,
		                                 const std::string &) = 0;
		virtual JournalSink &newNullSink(const std::string &) = 0;
		virtual JournalSink &appendSink(const std::string &,
		                                std::shared_ptr<JournalSink>) = 0;
		virtual void dropSink(const std::string &) = 0;
	};

	class JournalSession {
	 public:
		virtual ~JournalSession() = default;
		virtual JournalLogger &getLogger() = 0;
		virtual JournalLoggerController &getController() = 0;
	};

	class Journal {
	 public:
		virtual ~Journal() = default;
		virtual JournalLogger &getSession() = 0;
		virtual void closeSession() = 0;
	};

	class ConfigurableJournal : public Journal {
	 public:
		virtual JournalLoggerController &getSessionController() = 0;
	};
}  // namespace CalX

#endif