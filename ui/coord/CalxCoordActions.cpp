
#include "ui/coord/CalxCoordActions.h"
#include "ui/CalxErrorHandler.h"

namespace CalXUI {

  CalxCoordActionMove::CalxCoordActionMove(CoordHandle *handle,
										   coord_point_t dest, double speed,
										   bool jump, bool relative)
	  : handle(handle),
		dest(dest),
		speed(speed),
		jump(jump),
		relative(relative) {}

  void CalxCoordActionMove::perform(SystemManager *sysman) {
	handle->open_session();
	if (relative) {
	  wxGetApp().getErrorHandler()->handle(
		  handle->getFloatPlane()->relativeMove(dest, speed, jump));
	} else {
	  wxGetApp().getErrorHandler()->handle(
		  handle->getFloatPlane()->move(dest, speed, jump));
	}
	handle->close_session();
  }

  void CalxCoordActionMove::stop() {
	handle->stop();
  }

  CalxCoordActionArc::CalxCoordActionArc(CoordHandle *handle,
										 coord_point_t dest, coord_point_t cen,
										 int splitter, double speed,
										 bool clockwise, bool relative)
	  : handle(handle),
		dest(dest),
		cen(cen),
		splitter(splitter),
		speed(speed),
		clockwise(clockwise),
		relative(relative) {}

  void CalxCoordActionArc::perform(SystemManager *sysman) {
	handle->open_session();
	if (relative) {
	  wxGetApp().getErrorHandler()->handle(handle->getFloatPlane()->relativeArc(
		  dest, cen, splitter, speed, clockwise));
	} else {
	  wxGetApp().getErrorHandler()->handle(
		  handle->getFloatPlane()->arc(dest, cen, splitter, speed, clockwise));
	}
	handle->close_session();
  }

  void CalxCoordActionArc::stop() {
	handle->stop();
  }

  CalxCoordActionCalibrate::CalxCoordActionCalibrate(CoordHandle *handle,
													 TrailerId tr)
	  : handle(handle), trailer(tr) {}

  void CalxCoordActionCalibrate::perform(SystemManager *sysman) {
	handle->open_session();
	wxGetApp().getErrorHandler()->handle(handle->calibrate(trailer));
	handle->close_session();
  }
  void CalxCoordActionCalibrate::stop() {
	this->handle->stop();
  }

  CalxCoordActionMeasure::CalxCoordActionMeasure(CoordHandle *handle,
												 TrailerId tr)
	  : handle(handle), trailer(tr) {}

  void CalxCoordActionMeasure::perform(SystemManager *sysman) {
	handle->open_session();
	wxGetApp().getErrorHandler()->handle(handle->measure(trailer));
	handle->close_session();
  }
  void CalxCoordActionMeasure::stop() {
	this->handle->stop();
  }

  CalxCoordActionConfigure::CalxCoordActionConfigure(CoordHandle *handle,
													 CalxCoordController *ctrl,
													 coord_point_t dest,
													 double speed)
	  : handle(handle),
		controller(ctrl),
		dest(dest),
		speed(speed),
		work(false) {}

  void CalxCoordActionConfigure::perform(SystemManager *sysman) {
	handle->open_session();
	work = true;
	motor_point_t offset = { 0, 0 };
	controller->setOffset(offset);
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
	  controller->setOffset(handle->getPosition());
	}
	wxGetApp().getErrorHandler()->handle(errcode);
	handle->close_session();
  }

  void CalxCoordActionConfigure::stop() {
	this->work = false;
	handle->stop();
  }

  CalxCoordActionGraphBuild::CalxCoordActionGraphBuild(CoordHandle *handle,
													   CoordTranslator *trans,
													   GraphBuilder *builder,
													   double speed)
	  : handle(handle), translator(trans), builder(builder), speed(speed) {
	this->state.plane = nullptr;
	this->state.work = false;
  }

  CalxCoordActionGraphBuild::~CalxCoordActionGraphBuild() {
	delete this->builder;
  }

  void CalxCoordActionGraphBuild::perform(SystemManager *sysman) {
	handle->open_session();
	wxGetApp().getErrorHandler()->handle(builder->floatBuild(
		sysman, handle->getFloatPlane(), translator, speed, &state));
	handle->close_session();
  }

  void CalxCoordActionGraphBuild::stop() {
	this->state.stop();
  }

  CalxCoordActionGraphPreview::CalxCoordActionGraphPreview(
	  CalxVirtualPlaneDialog *dialog, CoordTranslator *trans,
	  GraphBuilder *builder, double speed)
	  : dialog(dialog), translator(trans), builder(builder), speed(speed) {
	this->state.plane = nullptr;
	this->state.work = false;
  }

  CalxCoordActionGraphPreview::~CalxCoordActionGraphPreview() {
	delete this->builder;
  }

  void CalxCoordActionGraphPreview::perform(SystemManager *sysman) {
	dialog->Enable(false);
	wxGetApp().getErrorHandler()->handle(builder->floatBuild(
		sysman, dialog->getFloatPlane(), translator, speed, &state));
	;
	dialog->Refresh();
	dialog->Enable(true);
  }

  void CalxCoordActionGraphPreview::stop() {
	this->state.stop();
  }
}
