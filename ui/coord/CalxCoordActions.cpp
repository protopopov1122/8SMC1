
#include "ui/coord/CalxCoordActions.h"
#include "ui/CalxErrorHandler.h"

namespace CalXUI {

	CalxCoordActionMove::CalxCoordActionMove(std::shared_ptr<CoordHandle> handle,
	                                         coord_point_t dest, double speed,
	                                         bool jump, bool relative,
	                                         ActionResult *act_res)
	    : handle(handle),
	      dest(dest),
	      speed(speed),
	      jump(jump),
	      relative(relative),
	      action_result(act_res) {
		if (this->action_result != nullptr) {
			this->action_result->ready = false;
			this->action_result->stopped = false;
			this->action_result->errcode = ErrorCode::NoError;
		}
	}

	void CalxCoordActionMove::perform(SystemManager *sysman) {
		ErrorCode errcode;
		handle->open_session();
		if (relative) {
			errcode = handle->getFloatPlane()->relativeMove(dest, speed, jump);
		} else {
			errcode = handle->getFloatPlane()->move(dest, speed, jump);
		}
		wxGetApp().getErrorHandler()->handle(errcode);
		handle->close_session();
		if (this->action_result != nullptr) {
			this->action_result->errcode = errcode;
			this->action_result->ready = true;
		}
	}

	void CalxCoordActionMove::stop() {
		handle->stop();
		if (this->action_result != nullptr) {
			this->action_result->stopped = true;
		}
	}

	CalxCoordActionArc::CalxCoordActionArc(std::shared_ptr<CoordHandle> handle,
	                                       coord_point_t dest, coord_point_t cen,
	                                       int splitter, double speed,
	                                       bool clockwise, bool relative,
	                                       ActionResult *act_res)
	    : handle(handle),
	      dest(dest),
	      cen(cen),
	      splitter(splitter),
	      speed(speed),
	      clockwise(clockwise),
	      relative(relative),
	      action_result(act_res) {
		if (this->action_result != nullptr) {
			this->action_result->ready = false;
			this->action_result->stopped = false;
			this->action_result->errcode = ErrorCode::NoError;
		}
	}

	void CalxCoordActionArc::perform(SystemManager *sysman) {
		ErrorCode errcode;
		handle->open_session();
		if (relative) {
			errcode = handle->getFloatPlane()->relativeArc(dest, cen, splitter, speed,
			                                               clockwise);
		} else {
			errcode =
			    handle->getFloatPlane()->arc(dest, cen, splitter, speed, clockwise);
		}
		wxGetApp().getErrorHandler()->handle(errcode);
		handle->close_session();
		if (this->action_result != nullptr) {
			this->action_result->errcode = errcode;
			this->action_result->ready = true;
		}
	}

	void CalxCoordActionArc::stop() {
		handle->stop();
		if (this->action_result != nullptr) {
			this->action_result->stopped = true;
		}
	}

	CalxCoordActionCalibrate::CalxCoordActionCalibrate(
	    std::shared_ptr<CoordHandle> handle, TrailerId tr, ActionResult *act_res)
	    : handle(handle), trailer(tr), action_result(act_res) {
		
		if (this->action_result != nullptr) {
			this->action_result->ready = false;
			this->action_result->stopped = false;
			this->action_result->errcode = ErrorCode::NoError;
		}
	}

	void CalxCoordActionCalibrate::perform(SystemManager *sysman) {
		handle->open_session();
		ErrorCode errcode = handle->calibrate(trailer);
		wxGetApp().getErrorHandler()->handle(errcode);
		handle->close_session();
		if (this->action_result != nullptr) {
			this->action_result->errcode = errcode;
			this->action_result->ready = true;
		}
	}
	void CalxCoordActionCalibrate::stop() {
		this->handle->stop();
		if (this->action_result != nullptr) {
			this->action_result->stopped = true;
		}
	}

	CalxCoordActionMeasure::CalxCoordActionMeasure(
	    std::shared_ptr<CoordHandle> handle, TrailerId tr, ActionResult *act_res)
	    : handle(handle), trailer(tr), action_result(act_res) {
		
		if (this->action_result != nullptr) {
			this->action_result->ready = false;
			this->action_result->stopped = false;
			this->action_result->errcode = ErrorCode::NoError;
		}
	}

	void CalxCoordActionMeasure::perform(SystemManager *sysman) {
		handle->open_session();
		ErrorCode errcode = handle->measure(trailer);
		wxGetApp().getErrorHandler()->handle(errcode);
		handle->close_session();
		if (this->action_result != nullptr) {
			this->action_result->errcode = errcode;
			this->action_result->ready = true;
		}
	}
	void CalxCoordActionMeasure::stop() {
		this->handle->stop();
		if (this->action_result != nullptr) {
			this->action_result->stopped = true;
		}
	}

	CalxCoordActionConfigure::CalxCoordActionConfigure(
	    std::shared_ptr<CoordHandle> handle, CalxCoordController *ctrl,
	    coord_point_t dest, double speed, ActionResult *act_res)
	    : handle(handle),
	      controller(ctrl),
	      dest(dest),
	      speed(speed),
	      work(false),
		  action_result(act_res) {
		
		if (this->action_result != nullptr) {
			this->action_result->ready = false;
			this->action_result->stopped = false;
			this->action_result->errcode = ErrorCode::NoError;
		}
	}

	void CalxCoordActionConfigure::perform(SystemManager *sysman) {
		handle->open_session();
		work = true;
		motor_point_t offset = { 0, 0 };
		controller->setOffset(offset);
		ErrorCode errcode = this->handle->measure(TrailerId::Trailer1);
		if (errcode != ErrorCode::NoError) {
			work = false;
		}
		coord_rect_t size = this->handle->getFloatPlane()->getFloatSize();
		double x =
		    static_cast<double>((((double) size.w) * this->dest.x) + size.x);
		double y =
		    static_cast<double>((((double) size.h) * this->dest.y) + size.y);
		coord_point_t dest = { x, y };
		if (work) {
			errcode = handle->getFloatPlane()->move(dest, speed, false);
		}
		if (work && errcode == ErrorCode::NoError) {
			controller->setOffset(handle->getPosition());
		}
		wxGetApp().getErrorHandler()->handle(errcode);
		handle->close_session();
		if (this->action_result != nullptr) {
			this->action_result->errcode = errcode;
			this->action_result->ready = true;
		}
	}

	void CalxCoordActionConfigure::stop() {
		this->work = false;
		handle->stop();
		if (this->action_result != nullptr) {
			this->action_result->stopped = true;
		}
	}

	CalxCoordActionGraphBuild::CalxCoordActionGraphBuild(
	    std::shared_ptr<CoordHandle> handle,
	    std::shared_ptr<CoordTranslator> trans,
	    std::unique_ptr<GraphBuilder> builder, double speed)
	    : handle(handle),
	      translator(trans),
	      builder(std::move(builder)),
	      speed(speed) {
		this->state = std::make_shared<TaskState>();
		this->state->plane = nullptr;
		this->state->work = false;
	}

	void CalxCoordActionGraphBuild::perform(SystemManager *sysman) {
		handle->open_session();
		wxGetApp().getErrorHandler()->handle(builder->floatBuild(
		    sysman, handle->getFloatPlane(), translator, speed, state));
		handle->close_session();
	}

	void CalxCoordActionGraphBuild::stop() {
		this->state->stop();
	}

	CalxCoordActionGraphPreview::CalxCoordActionGraphPreview(
	    CalxVirtualPlaneDialog *dialog, std::shared_ptr<CoordTranslator> trans,
	    std::unique_ptr<GraphBuilder> builder, double speed)
	    : dialog(dialog),
	      translator(trans),
	      builder(std::move(builder)),
	      speed(speed) {
		this->state = std::make_shared<TaskState>();
		this->state->plane = nullptr;
		this->state->work = false;
	}

	void CalxCoordActionGraphPreview::perform(SystemManager *sysman) {
		dialog->Enable(false);
		wxGetApp().getErrorHandler()->handle(builder->floatBuild(
		    sysman, dialog->getFloatPlane(), translator, speed, state));
		;
		dialog->Refresh();
		dialog->Enable(true);
	}

	void CalxCoordActionGraphPreview::stop() {
		this->state->stop();
	}
}  // namespace CalXUI
