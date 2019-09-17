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

#include "calx/ctrl-lib/logger/Sink.h"
#include "calx/ctrl-lib/logger/Format.h"
#include "calx/ctrl-lib/logger/Filter.h"
#include <ctime>
#include <chrono>
#include <sstream>

namespace CalX {

	JournalAbstractSink::JournalAbstractSink(const std::string &name)
	    : name(name),
	      filter(LoggerFilter::severity_any()),
	      formatter(JournalFormatter::defaultFormat(name)) {}

	const std::string &JournalAbstractSink::getName() const {
		return this->name;
	}

	void JournalAbstractSink::log(const LogEntry &entry) const {
		if (this->filter(entry)) {
			std::stringstream out;
			this->formatter(entry, out);
			this->log(out.str());
		}
	}

	void JournalAbstractSink::setFilter(
	    std::function<bool(const LogEntry &)> filter) {
		this->filter = filter;
	}

	void JournalAbstractSink::setFormatter(
	    std::function<void(const LogEntry &, std::ostream &)> formatter) {
		this->formatter = formatter;
	}

	JournalStreamSink::JournalStreamSink(const std::string &name,
	                                     std::ostream &output)
	    : JournalAbstractSink::JournalAbstractSink(name), output(output) {}

	void JournalStreamSink::log(const std::string &msg) const {
		this->output << msg << std::flush;
	}

	JournalFileSink::JournalFileSink(const std::string &name,
	                                 const std::string &path)
	    : JournalAbstractSink::JournalAbstractSink(name),
	      output(std::make_unique<std::ofstream>(
	          path, std::ios::out | std::ios::app)) {}

	JournalFileSink::~JournalFileSink() {
		this->output->close();
		this->output = nullptr;
	}

	void JournalFileSink::log(const std::string &msg) const {
		*this->output << msg << std::flush;
	}

	JournalNullSink::JournalNullSink(const std::string &name) : name(name) {}

	const std::string &JournalNullSink::getName() const {
		return this->name;
	}

	void JournalNullSink::log(const LogEntry &entry) const {}

	void JournalNullSink::setFilter(
	    std::function<bool(const LogEntry &)> filter) {}

	void JournalNullSink::setFormatter(
	    std::function<void(const LogEntry &, std::ostream &)> formatter) {}

	JournalStreamSinkFactory::JournalStreamSinkFactory(std::ostream &stream)
	    : stream(stream) {}

	std::unique_ptr<JournalSink> JournalStreamSinkFactory::newSink(
	    const std::string &name) const {
		return std::make_unique<JournalStreamSink>(name, this->stream);
	}

	JournalFileSinkFactory::JournalFileSinkFactory(const std::string &file)
	    : file(file) {}

	std::unique_ptr<JournalSink> JournalFileSinkFactory::newSink(
	    const std::string &name) const {
		return std::make_unique<JournalFileSink>(name, this->file);
	}

	JournalSinkStream::JournalSinkStream(const Loggable &log,
	                                     LoggingSeverity severity,
	                                     const std::string &tag,
	                                     const SourcePosition &position)
	    : log(log), severity(severity), tag(tag), position(position) {}

	JournalSinkStream::~JournalSinkStream() {
		this->flush();
	}

	void JournalSinkStream::flush() {
		std::string content(this->buffer.str());
		if (!content.empty()) {
			this->buffer.str("");
			LogEntry entry(this->severity, content, this->tag, this->position);
			this->log.log(entry);
		}
	}

	JournalSinkStream JournalSink::stream(LoggingSeverity severity,
	                                      const std::string &tag,
	                                      const SourcePosition &position) const {
		return JournalSinkStream(*this, severity, tag, position);
	}
}  // namespace CalX