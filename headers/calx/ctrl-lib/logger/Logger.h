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

#ifndef CALX_CTRL_LIB_LOGGER_LOGGER_H_
#define CALX_CTRL_LIB_LOGGER_LOGGER_H_

#include "calx/ctrl-lib/logger/Journal.h"

namespace CalX {

	class DefaultJournal : public ConfigurableJournal {
	 public:
		DefaultJournal(LoggingSeverity = LoggingSeverity::Debug);
		JournalLogger &getSession() override;
		void closeSession() override;
		JournalLoggerController &getSessionController() override;

	 private:
		JournalSession &openSession();
		std::unique_ptr<JournalSession> session;
		LoggingSeverity defaultSeverity;
	};
}  // namespace CalX

#endif