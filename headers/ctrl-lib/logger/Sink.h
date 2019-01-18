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

#ifndef CALX_CTRL_LIB_LOGGER_SINK_H_
#define CALX_CTRL_LIB_LOGGER_SINK_H_

#include "ctrl-lib/logger/Journal.h"
#include <iosfwd>
#include <fstream>

namespace CalX {

	class JournalAbstractSink : public JournalSink {
	 public:
		JournalAbstractSink(const std::string &);
		const std::string &getName() const override;
		void log(const LogEntry &) const override;
		void setFilter(std::function<bool(const LogEntry &)>) override;
		void setFormatter(
		    std::function<void(const LogEntry &, std::ostream &)>) override;

	 protected:
		virtual void log(const std::string &) const = 0;
		static constexpr unsigned int SinkNamePadding = 10;

		std::string name;
		std::function<bool(const LogEntry &)> filter;
		std::function<void(const LogEntry &, std::ostream &)> formatter;
	};

	class JournalStreamSink : public JournalAbstractSink {
	 public:
		JournalStreamSink(const std::string &, std::ostream &);

	 protected:
		void log(const std::string &) const override;

	 private:
		std::ostream &output;
	};

	class JournalFileSink : public JournalAbstractSink {
	 public:
		JournalFileSink(const std::string &, const std::string &);
		virtual ~JournalFileSink();

	 protected:
		void log(const std::string &) const override;

	 private:
		std::unique_ptr<std::ofstream> output;
	};

	class JournalNullSink : public JournalSink {
	 public:
		JournalNullSink(const std::string &);
		const std::string &getName() const override;
		void log(const LogEntry &) const override;
		void setFilter(std::function<bool(const LogEntry &)>) override;
		void setFormatter(
		    std::function<void(const LogEntry &, std::ostream &)>) override;

	 private:
		std::string name;
	};

	class JournalStreamSinkFactory : public JournalSinkFactory {
	 public:
		JournalStreamSinkFactory(std::ostream &);

		std::unique_ptr<JournalSink> newSink(const std::string &) const override;

	 private:
		std::ostream &stream;
	};

	class JournalFileSinkFactory : public JournalSinkFactory {
	 public:
		JournalFileSinkFactory(const std::string &);

		std::unique_ptr<JournalSink> newSink(const std::string &) const override;

	 private:
		std::string file;
	};
}  // namespace CalX

#endif