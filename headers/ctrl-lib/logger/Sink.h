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

#ifndef CALX_CTRL_LIB_LOGGER_SINK_H_
#define CALX_CTRL_LIB_LOGGER_SINK_H_

#include "ctrl-lib/logger/Journal.h"
#include <iosfwd>

namespace CalX {

  class JournalStreamSink : public JournalSink {
   public:
    JournalStreamSink(const std::string &, LoggingSeverity, std::ostream &);
    const std::string &getName() const override;
    void log(LoggingSeverity, const std::string &, const std::string &, const SourcePosition &) const override;
    LoggingSeverity getLevel() const override;
    void setLevel(LoggingSeverity) override;
   private:
    static constexpr unsigned int SinkNamePadding = 10;

    std::string name;
    LoggingSeverity severity;
    std::ostream &output;
  };

  class JournalNullSink : public JournalSink {
   public:
    JournalNullSink(const std::string &);
    const std::string &getName() const override;
    void log(LoggingSeverity, const std::string &, const std::string &, const SourcePosition &) const override;
    LoggingSeverity getLevel() const override;
    void setLevel(LoggingSeverity) override;
   private:
    std::string name;
  };

  class JournalStreamSinkFactory : public JournalSinkFactory {
   public:
    JournalStreamSinkFactory(std::ostream &);

    std::unique_ptr<JournalSink> newSink(const std::string &, LoggingSeverity) const override;
   private:
    std::ostream &stream;
  };
}

#endif