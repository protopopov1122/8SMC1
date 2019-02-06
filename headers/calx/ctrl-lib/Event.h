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

#ifndef CALX_CTRL_LIB_EVENT_H_
#define CALX_CTRL_LIB_EVENT_H_

#include "calx/ctrl-lib/CtrlCore.h"
#include <vector>
#include <algorithm>

namespace CalX {

	template<typename T>
	class AbstractEventSource {
	 public:
		virtual ~AbstractEventSource() = default;
		virtual void addEventListener(T) = 0;
		virtual void removeEventListener(T) = 0;
	};

	template<typename T>
	class EventSource : public AbstractEventSource<T> {
	 public:
		void addEventListener(T listener) override {
			this->listeners.push_back(listener);
		}
		void removeEventListener(T listener) override {
			this->listeners.erase(
			    std::remove(this->listeners.begin(), this->listeners.end(), listener),
			    this->listeners.end());
		}

	 protected:
		template<typename F, typename... E>
		void submitEvent(F method, E... evt) const {
			for (const auto &listener : this->listeners) {
				((*listener).*method)(evt...);
			}
		}

		std::vector<T> listeners;
	};
}  // namespace CalX

#endif