
#include "ui/coord/CalxCoordActions.h"
#include "ui/CalxErrorHandler.h"

namespace CalXUI {
	
	CalxCoordActionMove::CalxCoordActionMove(CalxLockableComponent *lockable, CoordHandle *handle,
		coord_point_t dest, double speed, bool jump, bool relative)
		: lockable(lockable), handle(handle), dest(dest), speed(speed), jump(jump), relative(relative) {}
	
	void CalxCoordActionMove::perform(SystemManager *sysman) {
		handle->open_session();
		lockable->setMaster(true);
		if (relative) {
		  wxGetApp().getErrorHandler()->handle(
			  handle->getFloatPlane()->relativeMove(dest, speed, jump));
		} else {
		  wxGetApp().getErrorHandler()->handle(
			  handle->getFloatPlane()->move(dest, speed, jump));
		}
		lockable->setMaster(false);
		handle->close_session();	
	}
	
	void CalxCoordActionMove::stop() {
		handle->stop();
	}
	
	CalxCoordActionArc::CalxCoordActionArc(CalxLockableComponent *lockable, CoordHandle *handle,
		coord_point_t dest, coord_point_t cen, int splitter, double speed, bool clockwise, bool relative)
		: lockable(lockable), handle(handle), dest(dest), cen(cen), splitter(splitter),
			speed(speed), clockwise(clockwise), relative(relative) {}
			
	void CalxCoordActionArc::perform(SystemManager *sysman) {
		handle->open_session();
		lockable->setMaster(true);
		if (relative) {
		  wxGetApp().getErrorHandler()->handle(
			  handle->getFloatPlane()->relativeArc(dest, cen, splitter, speed, clockwise));
		} else {
		  wxGetApp().getErrorHandler()->handle(
			  handle->getFloatPlane()->arc(dest, cen, splitter, speed, clockwise));
		}
		lockable->setMaster(false);
		handle->close_session();
	}
	
	void CalxCoordActionArc::stop() {
		handle->stop();
	}
	
	CalxCoordActionCalibrate::CalxCoordActionCalibrate(CalxLockableComponent *lockable, CoordHandle *handle, TrailerId tr)
		: lockable(lockable), handle(handle), trailer(tr) {}
	
	void CalxCoordActionCalibrate::perform(SystemManager *sysman) {
		handle->open_session();
		lockable->setMaster(true);
		wxGetApp().getErrorHandler()->handle(handle->calibrate(trailer));
		lockable->setMaster(false);
		handle->close_session();	
	}
	void CalxCoordActionCalibrate::stop() {
		this->handle->stop();
	}
	
	CalxCoordActionMeasure::CalxCoordActionMeasure(CalxLockableComponent *lockable, CoordHandle *handle, TrailerId tr)
		: lockable(lockable), handle(handle), trailer(tr) {}
	
	void CalxCoordActionMeasure::perform(SystemManager *sysman) {
		handle->open_session();
		lockable->setMaster(true);
		wxGetApp().getErrorHandler()->handle(handle->measure(trailer));
		lockable->setMaster(false);
		handle->close_session();	
	}
	void CalxCoordActionMeasure::stop() {
		this->handle->stop();
	}
	
	CalxCoordActionConfigure::CalxCoordActionConfigure(CalxLockableComponent *lockable, CoordHandle *handle,
		CalxFilterController *filters, coord_point_t dest, double speed) 
		: lockable(lockable), handle(handle), filters(filters), dest(dest), speed(speed), work(false) {}
	
	void CalxCoordActionConfigure::perform(SystemManager *sysman) {
		handle->open_session();
		work = true;
		lockable->setMaster(true);
		motor_point_t offset = { 0, 0 };
		filters->setOffset(offset);
		ErrorCode errcode = this->handle->measure(TrailerId::Trailer1);
		if (errcode != ErrorCode::NoError) {
			work = false;
		}
		motor_rect_t size = this->handle->getSize();
		motor_coord_t x =
		  (motor_coord_t)(((double) size.w) * this->dest.x) + size.x;
		motor_coord_t y =
		  (motor_coord_t)(((double) size.h) * this->dest.y) + size.y;
		motor_point_t dest = { x, y };
		if (work) {
			errcode = handle->move(dest, speed, false);
		}
		if (work && errcode == ErrorCode::NoError) {
			filters->setOffset(handle->getPosition());;
		}
		wxGetApp().getErrorHandler()->handle(errcode);
		lockable->setMaster(false);
		handle->close_session();
	}
	
	void CalxCoordActionConfigure::stop() {
		this->work = false;
		handle->stop();	
	}
	
	CalxCoordActionGraphBuild::CalxCoordActionGraphBuild(CalxLockableComponent *lockable, CoordHandle *handle,
		CoordTranslator *trans, GraphBuilder *builder, double speed)
		: lockable(lockable), handle(handle), translator(trans), builder(builder), speed(speed) {
		this->state.plane = nullptr;
		this->state.work = false;
	}
	
	CalxCoordActionGraphBuild::~CalxCoordActionGraphBuild() {
		delete this->builder;
	}
	
	void CalxCoordActionGraphBuild::perform(SystemManager *sysman) {
		handle->open_session();
		lockable->setMaster(true);
		wxGetApp().getErrorHandler()->handle(builder->floatBuild(
			sysman, handle->getFloatPlane(), translator, speed, &state));
		lockable->setMaster(false);
		handle->close_session();
	}
	
	void CalxCoordActionGraphBuild::stop() {
		this->state.stop();
	}
}