#ifndef CALX_CTRL_LIB_EVENT_H_
#define CALX_CTRL_LIB_EVENT_H_

#include "ctrl-lib/CtrlCore.h"
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
			this->listeners.erase(std::remove(this->listeners.begin(), this->listeners.end(), listener), this->listeners.end());
		}
	 protected:
		template <typename F, typename... E>
		void submitEvent(F method, E... evt) const {
			for (const auto &listener : this->listeners) {
				((*listener).*method)(evt...);
			}
		}

		std::vector<T> listeners;
	};
}

#endif