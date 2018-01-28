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

#ifndef CALX_CTRL_LIB_PLANE_COORD_CONTROLLER_H_
#define CALX_CTRL_LIB_PLANE_COORD_CONTROLLER_H_

#include "ctrl-lib/ctrl/InstrumentController.h"
#include "ctrl-lib/ctrl/MotorController.h"
#include "ctrl-lib/plane/AbstractCoordPlane.h"

namespace CalX {

	class CoordController : public CoordPlane {
	 public:
		CoordController(ConfigManager &, std::shared_ptr<MotorController>,
		                std::shared_ptr<MotorController>,
		                std::shared_ptr<InstrumentController> = nullptr);
		std::shared_ptr<MotorController> getXAxis();
		std::shared_ptr<MotorController> getYAxis();
		std::shared_ptr<InstrumentController> getInstrument();

		virtual ErrorCode move(motor_point_t, float, bool);
		virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, bool,
		                      float = 1.0f);
		virtual ErrorCode calibrate(TrailerId);
		virtual ErrorCode measure(TrailerId);
		virtual motor_point_t getPosition();
		virtual motor_rect_t getSize();
		virtual bool isMeasured();
		virtual void stop();
		void kill();
		virtual void dump(std::ostream &);
		virtual std::unique_ptr<CoordPlane> clone(std::shared_ptr<CoordPlane>);
		virtual CoordPlaneStatus getStatus();
		virtual ErrorCode open_session();
		virtual ErrorCode close_session();

		void addEventListener(std::shared_ptr<CoordEventListener>);
		void removeEventListener(std::shared_ptr<CoordEventListener>);
		virtual bool isUsed();
		virtual void use();
		virtual void unuse();

	 private:
		std::shared_ptr<MotorController> xAxis;
		std::shared_ptr<MotorController> yAxis;
		std::shared_ptr<InstrumentController> instr;
		ConfigManager &config;
		bool work;
		bool defWork;
		bool session_opened;
		CoordPlaneStatus status;

		motor_rect_t size;
		bool measured;
		std::vector<std::shared_ptr<CoordEventListener>> listeners;

		void sendMovingEvent(CoordMoveEvent &);
		void sendMovedEvent(CoordMoveEvent &);
		void sendStoppedEvent(CoordErrorEvent &);
		void sendCalibratingEvent(CoordCalibrateEvent &);
		void sendCalibratedEvent(CoordCalibrateEvent &);
	};
}  // namespace CalX

#endif
