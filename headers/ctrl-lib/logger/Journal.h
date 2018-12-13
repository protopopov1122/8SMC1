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

namespace CalX {

  struct SourcePosition {
    SourcePosition() : file(""), line(0) {}
    SourcePosition(const std::string &file, unsigned int line) : file(file), line(line) {}
    SourcePosition(const char *file, unsigned int line) : file(file), line(line) {}
    SourcePosition(const SourcePosition &position) : file(position.file), line(position.line) {}
    bool isEmpty() const {
      return this->file.size() == 0 || this->line == 0;
    }

    const std::string file;
    const unsigned int line;
  };

  class JournalSink {
   public:
    virtual ~JournalSink() = default;
    virtual const std::string &getName() const = 0;
    virtual void log(LoggingSeverity, const std::string &, const std::string & = "", const SourcePosition & = SourcePosition()) const = 0;
    virtual LoggingSeverity getLevel() const = 0;
    virtual void setLevel(LoggingSeverity) = 0;
  };

  class JournalSinkFactory {
   public:
    virtual ~JournalSinkFactory() = default;
    virtual std::unique_ptr<JournalSink> newSink(const std::string &, LoggingSeverity) const = 0;
  };

  class JournalSession {
   public:
    virtual ~JournalSession() = default;
    virtual const JournalSink &getDefaultSink() const = 0;
    virtual const JournalSink &getSink(const std::string &) = 0;
    virtual void getSinks(std::vector<std::reference_wrapper<JournalSink>> &) const = 0;
  };
}

#endif