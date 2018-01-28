/*
        Copyright (c) 2017 Jevgenijs Protopopovs

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

/* This header contains event type defintions.
   CalX Ctrl-lib widely uses events to communicate
   with user interface. Events are based on possible
   action types and are emited by these actions */

namespace CalX {

	class ConfigManager;  // Forward referencing
	class ConfigEntry;    // Forward referencing

	struct ErrorEvent {
		ErrorEvent(ErrorCode e) : errcode(e) {}
		ErrorEvent(const ErrorEvent &e) : errcode(e.errcode) {}
		ErrorCode errcode;
	};

	struct MotorMoveEvent {
		MotorMoveEvent(motor_coord_t d, float s) : destination(d), speed(d) {}
		MotorMoveEvent(const MotorMoveEvent &e)
		    : destination(e.destination), speed(e.speed) {}
		motor_coord_t destination;
		float speed;
	};

	typedef ErrorEvent MotorErrorEvent;

	struct MotorRollEvent {
		MotorRollEvent(TrailerId tr) : trailer(tr) {}
		MotorRollEvent(const MotorRollEvent &e) : trailer(e.trailer) {}
		TrailerId trailer;
	};

	class MotorEventListener {
	 public:
		MotorEventListener() {
			INIT_LOG("MotorEventListener");
		}
		virtual ~MotorEventListener() {
			DESTROY_LOG("MotorEventListener");
		}
		virtual void use() {}
		virtual void unuse() {}
		virtual void moving(MotorMoveEvent &) {}
		virtual void moved(MotorMoveEvent &) {}
		virtual void stopped(MotorErrorEvent &) {}
		virtual void rolling(MotorRollEvent &) {}
		virtual void rolled(MotorRollEvent &) {}
	};

	class InstrumentEventListener {
	 public:
		InstrumentEventListener() {
			INIT_LOG("InstrumentEventListener");
		}
		virtual ~InstrumentEventListener() {
			DESTROY_LOG("InstrumentEventListener");
		}
		virtual void use() {}
		virtual void unuse() {}
		virtual void stateChanged(bool state, bool enabled) {}
	};

	struct CoordMoveEvent {
		CoordMoveEvent(motor_point_t p, float s, bool sy)
		    : destination(p), speed(s), synchrone(sy) {}
		CoordMoveEvent(const CoordMoveEvent &e)
		    : destination(e.destination), speed(e.speed), synchrone(e.synchrone) {}
		motor_point_t destination;
		float speed;
		bool synchrone;
	};

	typedef ErrorEvent CoordErrorEvent;
	typedef MotorRollEvent CoordCalibrateEvent;

	class CoordEventListener {
	 public:
		CoordEventListener() {
			INIT_LOG("CoordEventListener");
		}
		virtual ~CoordEventListener() {
			DESTROY_LOG("CoordEventListener");
		}
		virtual void use() {}
		virtual void unuse() {}
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
		virtual void keyAdded(ConfigManager *, ConfigEntry *, std::string) {}
		virtual void keyRemoved(ConfigManager *, ConfigEntry *, std::string) {}
		virtual void keyChanged(ConfigManager *, ConfigEntry *, std::string) {}
	};
}  // namespace CalX

#endif