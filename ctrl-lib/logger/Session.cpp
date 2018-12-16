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

#include "ctrl-lib/logger/Session.h"

namespace CalX {

	JournalSinkStream JournalLogger::stream(LoggingSeverity severity,
	                                        const std::string &tag,
	                                        const SourcePosition &position) {
		return JournalSinkStream(*this, severity, tag, position);
	}

	JournalDefaultLogger::JournalDefaultLogger()
	    : defaultSink(std::make_shared<JournalNullSink>("")),
	      filter([](const auto &entry) { return true; }) {}

	JournalSink &JournalDefaultLogger::getDefaultSink() {
		return *this->defaultSink;
	}

	JournalSink &JournalDefaultLogger::getSink(const std::string &name) {
		if (this->sinks.count(name)) {
			return *this->sinks.at(name);
		} else {
			return *this->defaultSink;
		}
	}

	void JournalDefaultLogger::getSinks(
	    std::vector<std::reference_wrapper<JournalSink>> &sinks) const {
		for (auto it = this->sinks.begin(); it != this->sinks.end(); ++it) {
			sinks.push_back(std::ref(*it->second));
		}
	}

	void JournalDefaultLogger::log(const LogEntry &entry) {
		if (this->filter(entry)) {
			for (auto it = this->sinks.begin(); it != this->sinks.end(); ++it) {
				it->second->log(entry);
			}
		}
	}

	JournalSink &JournalDefaultLogger::newStreamSink(const std::string &name,
	                                                 std::ostream &stream,
	                                                 bool makeDefault) {
		std::shared_ptr<JournalSink> sink =
		    std::make_shared<JournalStreamSink>(name, stream);
		this->sinks[name] = sink;
		if (makeDefault) {
			this->defaultSink = sink;
		}
		return *sink;
	}

	JournalSink &JournalDefaultLogger::newFileSink(const std::string &name,
	                                               const std::string &path,
	                                               bool makeDefault) {
		std::shared_ptr<JournalSink> sink =
		    std::make_shared<JournalFileSink>(name, path);
		this->sinks[name] = sink;
		if (makeDefault) {
			this->defaultSink = sink;
		}
		return *sink;
	}

	void JournalDefaultLogger::setFilter(
	    std::function<bool(const LogEntry &)> filter) {
		this->filter = filter;
	}

	void JournalDefaultLogger::setDefaultSink(const std::string &name) {
		if (this->sinks.count(name)) {
			this->defaultSink = this->sinks[name];
		}
	}
}  // namespace CalX