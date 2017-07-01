/*
    Copyright (c) 2017 Jevgenijs Protopopovs

    This file is part of CalX project.

    CalX is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CalX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef CALX_CTRL_LIB_PLANE_COORD_PLANE_LOG_H_
#define CALX_CTRL_LIB_PLANE_COORD_PLANE_LOG_H_

#include "ctrl-lib/plane/AbstractCoordPlane.h"

namespace CalX {

  class CoordPlaneLog : public CoordPlane {
  	public:
  		CoordPlaneLog(CoordPlane*, std::ostream*, std::string, bool = true, bool = true);
  		virtual ~CoordPlaneLog();
  		CoordPlane *getBase();
  		std::string getPrefix();
  		bool isLoggingActions();
  		bool isLoggingErrors();
  		void setPrefix(std::string);
  		void setLoggingActions(bool);
  		void setLoggingErrors(bool);

  		virtual ErrorCode move(motor_point_t, float, int, bool);
  		virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, float = 1.0f, bool = false);
  		virtual ErrorCode calibrate(TrailerId);
  		virtual ErrorCode measure(TrailerId);
  		virtual motor_point_t getPosition();
  		virtual motor_rect_t getSize();
  		virtual bool isMeasured();
  		virtual void use();
  		virtual void unuse();
  		virtual void stop();
  		virtual void dump(std::ostream&);
  		virtual CoordPlane *clone(CoordPlane*);
  		virtual CoordPlaneStatus getStatus();
  		virtual ErrorCode open_session();
  		virtual ErrorCode close_session();
  	private:
  		CoordPlane *plane;
  		std::ostream *out;
  		std::string prefix;
  		bool log_actions;
  		bool log_errors;
  };
}

#endif
