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

#ifndef CALX_CTRL_LIB_LOGGER_JOURNAL_H_
#define CALX_CTRL_LIB_LOGGER_JOURNAL_H_

#include "platform.h"
#include "ctrl-lib/logger/Level.h"
#include <string>
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

	class JournalSink;

	struct Flush {};

	class JournalSinkStream {
	 public:
		JournalSinkStream(JournalSink &, LoggingSeverity, const std::string & = "",
		                  const SourcePosition & = SourcePosition());
		~JournalSinkStream();

		void flush();

		JournalSinkStream &operator<<(Flush flush) {
			this->flush();
			return *this;
		}

		template<typename T>
		JournalSinkStream &operator<<(T value) {
			this->buffer << value;
			return *this;
		}

	 private:
		JournalSink &sink;
		LoggingSeverity severity;
		std::string tag;
		SourcePosition position;
		std::stringstream buffer;
	};

	class JournalSink {
	 public:
		virtual ~JournalSink() = default;
		virtual const std::string &getName() const = 0;
		virtual void log(LoggingSeverity, const std::string &,
		                 const std::string & = "",
		                 const SourcePosition & = SourcePosition()) = 0;
		virtual LoggingSeverity getLevel() const = 0;
		virtual void setLevel(LoggingSeverity) = 0;

		JournalSinkStream stream(const std::string & = "",
		                         const SourcePosition & = SourcePosition());
		JournalSinkStream stream(LoggingSeverity, const std::string & = "",
		                         const SourcePosition & = SourcePosition());

		template<typename T>
		JournalSinkStream operator<<(T value) {
			JournalSinkStream stream = this->stream();
			stream << value;
			return stream;
		}
	};

	class JournalSinkFactory {
	 public:
		virtual ~JournalSinkFactory() = default;
		virtual std::unique_ptr<JournalSink> newSink(const std::string &,
		                                             LoggingSeverity) const = 0;
	};

	class JournalLogger {
	 public:
		virtual ~JournalLogger() = default;
		virtual JournalSink &getDefaultSink() = 0;
		virtual JournalSink &getSink(const std::string &) = 0;
		virtual void getSinks(
		    std::vector<std::reference_wrapper<JournalSink>> &) const = 0;

		template<typename T>
		JournalSinkStream operator<<(T value) {
			return this->getDefaultSink() << value;
		}
	};

	class JournalLoggerController {
	 public:
		virtual ~JournalLoggerController() = default;
		virtual LoggingSeverity getDefaultSeverity() const = 0;
		virtual void setDefaultSeverity(LoggingSeverity) = 0;
		virtual JournalSink &newStreamSink(const std::string &, std::ostream &,
		                                   bool = false) = 0;
		virtual JournalSink &newFileSink(const std::string &, const std::string &,
		                                 bool = false) = 0;
		virtual void setDefaultSink(const std::string &) = 0;
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