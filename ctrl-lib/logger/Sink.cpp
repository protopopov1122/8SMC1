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

  JournalStreamSink::JournalStreamSink(const std::string &name, LoggingSeverity severity, std::ostream &output)
    : name(name), severity(severity), output(output) {}
  
  const std::string &JournalStreamSink::getName() const {
    return this->name;
  }

  void JournalStreamSink::log(LoggingSeverity severity, const std::string &message, const std::string &tag, const SourcePosition &position) const {
    if (static_cast<int>(severity) < static_cast<int>(this->severity)) {
      return;
    }

    std::stringstream out;
    std::string fullName(this->name);
    if (fullName.size() < JournalStreamSink::SinkNamePadding) {
      fullName.insert(fullName.size(), JournalStreamSink::SinkNamePadding - fullName.size(), ' ');
    }
    auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string currentTime(ctime(&timenow));
    out << fullName << currentTime.substr(0, currentTime.size() - 1) << '\t';

    std::string fullTag;
    if (tag.size() > 0) {
      fullTag.append(" [");
      fullTag.append(tag);
      fullTag.push_back(']');
    }
    if (!position.isEmpty()) {
      fullTag.append("@(");
      fullTag.append(position.file);
      fullTag.push_back(':');
      fullTag.append(std::to_string(position.line));
      fullTag.push_back(')');
    }

    if (fullTag.size() > 0) {
      if (fullTag.size() < 50) {
        fullTag.insert(fullTag.size(), 50 - fullTag.size(), ' ');
      }
      out << fullTag << '\t' << message << std::endl;
    } else {
      out << message << std::endl;
    }
    this->output << out.str() << std::flush;
  }

  LoggingSeverity JournalStreamSink::getLevel() const {
    return this->severity;
  }

  void JournalStreamSink::setLevel(LoggingSeverity severity) {
    this->severity = severity;
  }

  JournalNullSink::JournalNullSink(const std::string &name)
    : name(name) {}
  
  const std::string &JournalNullSink::getName() const {
    return this->name;
  }

  void JournalNullSink::log(LoggingSeverity severity, const std::string &message, const std::string &tag, const SourcePosition &position) const {
  }

  LoggingSeverity JournalNullSink::getLevel() const {
    return LoggingSeverity::Debug;
  }

  void JournalNullSink::setLevel(LoggingSeverity severity) {
  }

  JournalStreamSinkFactory::JournalStreamSinkFactory(std::ostream &stream)
    : stream(stream) {}
  
  std::unique_ptr<JournalSink> JournalStreamSinkFactory::newSink(const std::string & name, LoggingSeverity severity) const {
    return std::make_unique<JournalStreamSink>(name, severity, this->stream);
  }
}