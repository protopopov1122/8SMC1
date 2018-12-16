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

#include "ctrl-lib/logger/Sink.h"
#include <ctime>
#include <chrono>
#include <sstream>
#include <iostream>

namespace CalX {

	JournalAbstractSink::JournalAbstractSink(const std::string &name)
	    : name(name), filter([](const auto &entry) { return true; }) {}

	const std::string &JournalAbstractSink::getName() const {
		return this->name;
	}

	void JournalAbstractSink::log(const LogEntry &entry) {
		if (!this->filter(entry)) {
			return;
		}

		std::stringstream out;
		std::string fullName(this->name);
		if (fullName.size() < JournalAbstractSink::SinkNamePadding) {
			fullName.insert(fullName.size(),
			                JournalStreamSink::SinkNamePadding - fullName.size(),
			                ' ');
		}
		auto timenow =
		    std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::string currentTime(ctime(&timenow));
		out << fullName << currentTime.substr(0, currentTime.size() - 1) << '\t';

		std::string fullTag;
		if (entry.tag.size() > 0) {
			fullTag.append(" [");
			fullTag.append(entry.tag);
			fullTag.push_back(']');
		}
		if (!entry.position.isEmpty()) {
			fullTag.append("@(");
			fullTag.append(entry.position.file);
			fullTag.push_back(':');
			fullTag.append(std::to_string(entry.position.line));
			fullTag.push_back(')');
		}

		if (fullTag.size() > 0) {
			if (fullTag.size() < 50) {
				fullTag.insert(fullTag.size(), 50 - fullTag.size(), ' ');
			}
			out << fullTag << '\t' << entry.message << std::endl;
		} else {
			out << entry.message << std::endl;
		}
		this->log(out.str());
	}

	void JournalAbstractSink::setFilter(
	    std::function<bool(const LogEntry &)> filter) {
		this->filter = filter;
	}

	JournalStreamSink::JournalStreamSink(const std::string &name,
	                                     std::ostream &output)
	    : JournalAbstractSink::JournalAbstractSink(name), output(output) {}

	void JournalStreamSink::log(const std::string &msg) {
		this->output << msg << std::flush;
	}

	JournalFileSink::JournalFileSink(const std::string &name,
	                                 const std::string &path)
	    : JournalAbstractSink::JournalAbstractSink(name),
	      output(path, std::ios::out | std::ios::app) {}

	JournalFileSink::~JournalFileSink() {
		this->output.close();
	}

	void JournalFileSink::log(const std::string &msg) {
		this->output << msg << std::flush;
	}

	JournalNullSink::JournalNullSink(const std::string &name) : name(name) {}

	const std::string &JournalNullSink::getName() const {
		return this->name;
	}

	void JournalNullSink::log(const LogEntry &entry) {}

	void JournalNullSink::setFilter(
	    std::function<bool(const LogEntry &)> filter) {}

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

	JournalSinkStream::JournalSinkStream(Loggable &log, LoggingSeverity severity,
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
	                                      const SourcePosition &position) {
		return JournalSinkStream(*this, severity, tag, position);
	}
}  // namespace CalX