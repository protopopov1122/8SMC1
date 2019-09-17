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

#include "calx/ctrl-lib/logger/Session.h"

namespace CalX {

	JournalSinkStream JournalLogger::stream(
	    LoggingSeverity severity, const std::string &tag,
	    const SourcePosition &position) const {
		return JournalSinkStream(*this, severity, tag, position);
	}

	JournalDefaultLogger::JournalDefaultLogger()
	    : filter([](const auto &entry) { return true; }) {}

	const JournalSink *JournalDefaultLogger::getSink(
	    const std::string &name) const {
		if (this->sinks.count(name)) {
			return this->sinks.at(name).get();
		} else {
			return nullptr;
		}
	}

	void JournalDefaultLogger::getSinks(
	    std::vector<std::reference_wrapper<const JournalSink>> &sinks) const {
		for (auto it = this->sinks.begin(); it != this->sinks.end(); ++it) {
			sinks.push_back(std::cref(*it->second));
		}
	}

	void JournalDefaultLogger::log(const LogEntry &entry) const {
		if (this->filter(entry)) {
			for (auto it = this->sinks.begin(); it != this->sinks.end(); ++it) {
				it->second->log(entry);
			}
		}
	}

	JournalSink &JournalDefaultLogger::newStreamSink(const std::string &name,
	                                                 std::ostream &stream) {
		std::shared_ptr<JournalSink> sink =
		    std::make_shared<JournalStreamSink>(name, stream);
		this->sinks[name] = sink;
		return *sink;
	}

	JournalSink &JournalDefaultLogger::newFileSink(const std::string &name,
	                                               const std::string &path) {
		std::shared_ptr<JournalSink> sink =
		    std::make_shared<JournalFileSink>(name, path);
		this->sinks[name] = sink;
		return *sink;
	}

	JournalSink &JournalDefaultLogger::newNullSink(const std::string &name) {
		std::shared_ptr<JournalSink> sink = std::make_shared<JournalNullSink>(name);
		this->sinks[name] = sink;
		return *sink;
	}

	JournalSink &JournalDefaultLogger::appendSink(
	    const std::string &name, std::shared_ptr<JournalSink> sink) {
		this->sinks[name] = sink;
		return *sink;
	}

	void JournalDefaultLogger::setFilter(
	    std::function<bool(const LogEntry &)> filter) {
		this->filter = filter;
	}

	void JournalDefaultLogger::dropSink(const std::string &name) {
		if (this->sinks.count(name) > 0) {
			this->sinks.erase(name);
		}
	}
}  // namespace CalX