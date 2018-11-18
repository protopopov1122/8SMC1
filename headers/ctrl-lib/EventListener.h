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

#ifndef CALX_CTRL_LIB_EVENT_LISTENER_H_
#define CALX_CTRL_LIB_EVENT_LISTENER_H_

#include "ctrl-lib/CtrlCore.h"
#include <string>
#include <algorithm>
#include <vector>

/* This header contains event type defintions.
   CalX Ctrl-lib widely uses events to communicate
   with user interface. Events are based on possible
   action types and are emited by these actions */

namespace CalX {

	class ConfigManager;  // Forward referencing
	class ConfigEntry;    // Forward referencing

	class UsageEventListener {
	 public:
		virtual ~UsageEventListener() = default;
		virtual void onUse() {}
		virtual void onUnuse() {}
	};

	struct ErrorEvent {
		ErrorCode errcode;
	};

	struct MotorMoveEvent {
		motor_coord_t destination;
		float speed;
	};

	typedef ErrorEvent MotorErrorEvent;

	struct MotorRollEvent {
		TrailerId trailer;
	};

	class MotorEventListener : public UsageEventListener {
	 public:
		MotorEventListener() {
			INIT_LOG("MotorEventListener");
		}
		virtual ~MotorEventListener() {
			DESTROY_LOG("MotorEventListener");
		}
		virtual void moving(MotorMoveEvent &) {}
		virtual void moved(MotorMoveEvent &) {}
		virtual void stopped(MotorErrorEvent &) {}
		virtual void rolling(MotorRollEvent &) {}
		virtual void rolled(MotorRollEvent &) {}
	};

	class InstrumentEventListener : public UsageEventListener {
	 public:
		InstrumentEventListener() {
			INIT_LOG("InstrumentEventListener");
		}
		virtual ~InstrumentEventListener() {
			DESTROY_LOG("InstrumentEventListener");
		}
		virtual void stateChanged(bool state, bool enabled) {}
	};

	struct CoordMoveEvent {
		motor_point_t destination;
		float speed;
		bool synchrone;
	};

	typedef ErrorEvent CoordErrorEvent;
	typedef MotorRollEvent CoordCalibrateEvent;

	class CoordEventListener : public UsageEventListener {
	 public:
		CoordEventListener() {
			INIT_LOG("CoordEventListener");
		}
		virtual ~CoordEventListener() {
			DESTROY_LOG("CoordEventListener");
		}
		virtual void moving(CoordMoveEvent &) {}
		virtual void moved(CoordMoveEvent &) {}
		virtual void stopped(CoordErrorEvent &) {}
		virtual void calibrating(CoordCalibrateEvent &) {}
		virtual void calibrated(CoordCalibrateEvent &) {}
	};

	class ConfigEventListener {
	 public:
		ConfigEventListener() {
			INIT_LOG("ConfigEventListener");
		}
		virtual ~ConfigEventListener() {
			DESTROY_LOG("ConfigEventListener");
		}
		virtual void entryAdded(ConfigManager *, std::string) {}
		virtual void entryRemoved(ConfigManager *, std::string) {}
		virtual void keyAdded(ConfigManager *, std::string, std::string) {}
		virtual void keyRemoved(ConfigManager *, std::string, std::string) {}
		virtual void keyChanged(ConfigManager *, std::string, std::string) {}
	};

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
}  // namespace CalX

#endif