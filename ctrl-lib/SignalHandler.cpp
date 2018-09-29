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

#include "ctrl-lib/SignalHandler.h"
#include <signal.h>

#ifdef OS_WIN
#include <windows.h>
#endif

CalX::SystemManager *SYSMAN = nullptr;

extern "C" void calx_terminate() {
	if (SYSMAN == nullptr) {
		return;
	}
	fprintf(stderr, "FATAL error. Terminating application\n");
	SYSMAN->getDeviceManager().terminate();
}

extern "C" void signal_handle(int signum) {
	calx_terminate();
	exit(-1);
}

#ifdef OS_WIN
LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS *ExceptionInfo) {
	calx_terminate();
	exit(-1);
	return EXCEPTION_EXECUTE_HANDLER;
}
#endif

namespace CalX {

	void setup_signals(SystemManager *sysman) {
		if (sysman != nullptr) {
			SYSMAN = sysman;
			signal(SIGABRT, signal_handle);
			signal(SIGINT, signal_handle);
			signal(SIGSEGV, signal_handle);
			signal(SIGFPE, signal_handle);
			signal(SIGILL, signal_handle);
			signal(SIGTERM, signal_handle);
#ifdef OS_WIN32
			SetUnhandledExceptionFilter(windows_exception_handler);
#endif
		}
	}
}  // namespace CalX
