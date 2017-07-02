#include "ctrl-lib/plane/CoordPlane.h"

namespace CalX {

	ErrorCode CoordPlane::relativeMove(motor_point_t relpoint, float speed, bool sync) {
		motor_point_t point = getPosition();
		point.x += relpoint.x;
		point.y += relpoint.y;
		return move(point, speed, sync);
	}

	ErrorCode CoordPlane::relativeArc(motor_point_t reldest, motor_point_t relcenter, int splitter,
				float speed, bool clockwise, float scale) {
		motor_point_t dest = getPosition();
		motor_point_t center = getPosition();
		dest.x += reldest.x;
		dest.y += reldest.y;
		center.x += relcenter.x;
		center.y += relcenter.y;
		return arc(dest, center, splitter, speed, clockwise, scale);
	}

  ErrorCode CoordPlane::open_session() {
    return ErrorCode::NoError;
  }

  ErrorCode CoordPlane::close_session() {
    return ErrorCode::NoError;
  }

  void TaskState::stop() {
	   plane->stop();
		 work = false;
  }
}
