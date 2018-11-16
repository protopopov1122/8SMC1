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

#include "ui/coord/CalxCoordAdjuster.h"
#include "ui/CalxErrorHandler.h"
#include <wx/dcbuffer.h>
#include <wx/sizer.h>

namespace CalXUI {

	CalxCoordAdjustAction::CalxCoordAdjustAction(
	    CalxCoordAdjustDialog *dialog, std::shared_ptr<CoordHandle> handle,
	    CalxAdjustDirection dir, double step, double speed)
	    : dialog(dialog),
	      handle(handle),
	      direction(dir),
	      step(step),
	      speed(speed) {}

	void CalxCoordAdjustAction::perform(SystemManager *sysman) {
		coord_point_t dest;
		switch (this->direction) {
			case CalxAdjustDirection::Up:
				dest = { 0, step };
				break;
			case CalxAdjustDirection::Down:
				dest = { 0, -step };
				break;
			case CalxAdjustDirection::Left:
				dest = { -step, 0 };
				break;
			case CalxAdjustDirection::Right:
				dest = { step, 0 };
				break;
		}

		dialog->setEnabled(false);
		handle->open_session();
		wxGetApp().getErrorHandler()->handle(
		    handle->getFloatPlane()->relativeMove(dest, speed, false));
		handle->close_session();
		dialog->setEnabled(true);
	}

	void CalxCoordAdjustAction::stop() {
		handle->stop();
	}

	CalxCoordAdjustMoveAction::CalxCoordAdjustMoveAction(
	    CalxCoordAdjustDialog *dialog, std::shared_ptr<CoordHandle> handle,
	    coord_point_t dest, double speed)
	    : dialog(dialog), handle(handle), dest(dest), speed(speed) {}

	void CalxCoordAdjustMoveAction::perform(SystemManager *sysman) {
		dialog->setEnabled(false);
		handle->open_session();
		wxGetApp().getErrorHandler()->handle(
		    handle->getFloatPlane()->move(dest, speed, false));
		handle->close_session();
		dialog->setEnabled(true);
	}

	void CalxCoordAdjustMoveAction::stop() {
		handle->stop();
	}

	CalxCoordAdjustRepaintTimer::CalxCoordAdjustRepaintTimer(
	    CalxCoordAdjustDialog *dialog)
	    : wxTimer::wxTimer(), dialog(dialog) {}

	void CalxCoordAdjustRepaintTimer::Notify() {
		this->dialog->updateUI();
	}

	CalxCoordAdjustViewer::CalxCoordAdjustViewer(
	    wxWindow *win, wxWindowID id, std::shared_ptr<CoordHandle> handle,
	    wxSize sz)
	    : wxWindow::wxWindow(win, id), handle(handle), pointer_colour(255, 0, 0) {
		SetMinSize(sz);
		ConfigEntry *colourEntry =
		    wxGetApp().getSystemManager()->getConfiguration().getEntry(
		        CalxConfiguration::Watcher);
		this->pointer_colour = wxColour(colourEntry->getInt(CalxWatcherConfiguration::PointerR, 255),
		                                colourEntry->getInt(CalxWatcherConfiguration::PointerG, 0),
		                                colourEntry->getInt(CalxWatcherConfiguration::PointerB, 0));
		this->Bind(wxEVT_PAINT, &CalxCoordAdjustViewer::OnPaintEvent, this);
		this->Bind(wxEVT_SIZE, &CalxCoordAdjustViewer::OnResizeEvent, this);
	}

	void CalxCoordAdjustViewer::OnPaintEvent(wxPaintEvent &evt) {
		wxPaintDC dc(this);
		wxBufferedDC buf(&dc, this->GetSize());
		render(buf);
	}

	void CalxCoordAdjustViewer::OnResizeEvent(wxSizeEvent &evt) {
		Refresh();
	}

	void CalxCoordAdjustViewer::render(wxDC &dc) {
		dc.SetBackground(*wxWHITE_BRUSH);
		dc.Clear();

		wxSize real_size = GetSize();
		motor_rect_t plane_size = this->handle->getController()->getSize();
		motor_rect_t top_plane_size = this->handle->getSize();
		coord_rect_t float_plane_size =
		    this->handle->getFloatPlane()->getFloatSize();
		double scaleX, scaleY, top_scaleX, top_scaleY;
		scaleX = ((double) real_size.x) / plane_size.w;
		scaleY = ((double) real_size.y) / plane_size.h;
		top_scaleX = ((double) real_size.x) / top_plane_size.w;
		top_scaleY = ((double) real_size.y) / top_plane_size.h;
		dc.SetPen(*wxBLUE_PEN);
		dc.DrawLine(0,
		            (wxCoord)((top_plane_size.h + top_plane_size.y) * top_scaleY),
		            real_size.x,
		            (wxCoord)((top_plane_size.h + top_plane_size.y) * top_scaleY));
		dc.DrawLine((wxCoord)((-top_plane_size.x) * top_scaleX), 0,
		            (wxCoord)((-top_plane_size.x) * top_scaleX), real_size.y);

		dc.SetPen(*wxBLACK_PEN);
		std::string units = wxGetApp().getUnitProcessor().getUnits();
		if (!units.empty()) {
			units = " " + units;
		}
		wxCoord x, y;
		dc.DrawText(
		    FORMAT("%s%s",
		           wxGetApp().getUnitProcessor().formatDouble(float_plane_size.x),
		           units),
		    0, (wxCoord)((top_plane_size.h + top_plane_size.y) * top_scaleY));
		dc.GetMultiLineTextExtent(
		    FORMAT("%s%s",
		           wxGetApp().getUnitProcessor().formatDouble(float_plane_size.x +
		                                                      float_plane_size.w),
		           units),
		    &x, &y);
		dc.DrawText(FORMAT("%s%s",
		                   wxGetApp().getUnitProcessor().formatDouble(
		                       float_plane_size.x + float_plane_size.w),
		                   units),
		            real_size.x - x,
		            (wxCoord)((top_plane_size.h + top_plane_size.y) * top_scaleY));
		dc.GetMultiLineTextExtent(
		    FORMAT("%s%s",
		           wxGetApp().getUnitProcessor().formatDouble(float_plane_size.y),
		           units),
		    &x, &y);
		dc.DrawText(
		    FORMAT("%s%s",
		           wxGetApp().getUnitProcessor().formatDouble(float_plane_size.y),
		           units),
		    (wxCoord)((-top_plane_size.x) * top_scaleX) - x / 2, real_size.y - y);
		dc.GetMultiLineTextExtent(
		    FORMAT("%s%s",
		           wxGetApp().getUnitProcessor().formatDouble(float_plane_size.y +
		                                                      float_plane_size.h),
		           units),
		    &x, &y);
		dc.DrawText(FORMAT("%s%s",
		                   wxGetApp().getUnitProcessor().formatDouble(
		                       float_plane_size.y + float_plane_size.h),
		                   units),
		            (wxCoord)((-top_plane_size.x) * top_scaleX) - x / 2, 0);

		wxPen pen(this->pointer_colour);
		wxBrush brush(this->pointer_colour);
		dc.SetPen(pen);
		dc.SetBrush(brush);
		motor_point_t point = this->handle->getController()->getPosition();
		double _x = ((double) (point.x - plane_size.x) * scaleX);
		double _y = ((double) (plane_size.h + plane_size.y - point.y) * scaleY);
		dc.DrawRectangle((int) _x - 2, (int) _y - 2, 4, 4);
	}

	CalxCoordAdjustDialog::CalxCoordAdjustDialog(
	    wxWindow *win, wxWindowID id, std::shared_ptr<CoordHandle> handle)
	    : wxDialog::wxDialog(win, id, __("Adjust dialog"), wxDefaultPosition,
	                         wxDefaultSize,
	                         wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
	      handle(handle) {
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);

		this->mouseCoords = new wxStaticText(this, wxID_ANY, __(""));
		sizer->Add(mouseCoords, 0, wxALL);

		this->viewer =
		    new CalxCoordAdjustViewer(this, wxID_ANY, handle, wxSize(500, 500));
		sizer->Add(viewer, 1, wxALL | wxEXPAND);

		prmsPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(prmsPanel, 0, wxALL | wxEXPAND);
		wxFlexGridSizer *prmsSizer = new wxFlexGridSizer(3);
		prmsPanel->SetSizer(prmsSizer);

		this->position = new wxStaticText(prmsPanel, wxID_ANY, "");
		this->stepSpin = new wxSpinCtrlDouble(
		    prmsPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, wxGetApp().getUnitProcessor().getUnitPrecision(),
		    INT_MAX,
		    wxGetApp()
		        .getSystemManager()
		        ->getConfiguration()
		        .getEntry(CalxConfiguration::UserInterface)
		        ->getReal(CalxUIConfiguration::AdjustStep, 2.5),
		    wxGetApp().getUnitProcessor().getUnitPrecision());
		this->speedSpin = new wxSpinCtrlDouble(
		    prmsPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, wxGetApp().getUnitProcessor().getSpeedPrecision(),
		    wxGetApp()
		        .getSystemManager()
		        ->getConfiguration()
		        .getEntry(CalxConfiguration::Units)
		        ->getReal(CalxUnitConfiguration::UnitSpeed, 4000.0),
		    wxGetApp()
		        .getSystemManager()
		        ->getConfiguration()
		        .getEntry(CalxConfiguration::Units)
		        ->getReal(CalxUnitConfiguration::UnitSpeed, 4000.0),
		    wxGetApp().getUnitProcessor().getSpeedPrecision());
		prmsSizer->Add(new wxStaticText(prmsPanel, wxID_ANY,
		                                __("Position") + std::string(":")),
		               0, wxALIGN_RIGHT | wxRIGHT, 5);
		prmsSizer->Add(this->position);
		prmsSizer->Add(new wxStaticText(prmsPanel, wxID_ANY, ""));
		prmsSizer->Add(
		    new wxStaticText(prmsPanel, wxID_ANY, __("Step") + std::string(":")), 0,
		    wxALIGN_RIGHT | wxRIGHT, 5);
		prmsSizer->Add(this->stepSpin, 0, wxALL | wxEXPAND);
		prmsSizer->Add(new wxStaticText(prmsPanel, wxID_ANY,
		                                wxGetApp().getUnitProcessor().getUnits()));
		prmsSizer->Add(
		    new wxStaticText(prmsPanel, wxID_ANY, __("Speed") + std::string(":")),
		    0, wxALIGN_RIGHT | wxRIGHT, 5);
		prmsSizer->Add(this->speedSpin, 0, wxALL | wxEXPAND);
		prmsSizer->Add(new wxStaticText(
		    prmsPanel, wxID_ANY, wxGetApp().getUnitProcessor().getSpeedUnits()));

		buttonPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(buttonPanel, 0, wxALL | wxALIGN_CENTER);
		wxFlexGridSizer *buttonSizer = new wxFlexGridSizer(3);
		buttonPanel->SetSizer(buttonSizer);
		wxButton *okButton = new wxButton(buttonPanel, wxID_ANY, __("OK"));
		stopButton = new wxButton(buttonPanel, wxID_ANY, __("Stop"));
		wxButton *leftButton = new wxButton(buttonPanel, wxID_ANY, __("Left"));
		wxButton *rightButton = new wxButton(buttonPanel, wxID_ANY, __("Right"));
		wxButton *upButton = new wxButton(buttonPanel, wxID_ANY, __("Up"));
		wxButton *downButton = new wxButton(buttonPanel, wxID_ANY, __("Down"));
		okButton->Bind(wxEVT_BUTTON, &CalxCoordAdjustDialog::OnOkClick, this);
		stopButton->Bind(wxEVT_BUTTON, &CalxCoordAdjustDialog::OnStopClick, this);
		leftButton->Bind(wxEVT_BUTTON, &CalxCoordAdjustDialog::OnLeftClick, this);
		rightButton->Bind(wxEVT_BUTTON, &CalxCoordAdjustDialog::OnRightClick, this);
		upButton->Bind(wxEVT_BUTTON, &CalxCoordAdjustDialog::OnUpClick, this);
		downButton->Bind(wxEVT_BUTTON, &CalxCoordAdjustDialog::OnDownClick, this);
		buttonSizer->Add(new wxStaticText(buttonPanel, wxID_ANY, ""));
		buttonSizer->Add(upButton, 1, wxALL | wxALIGN_CENTER);
		buttonSizer->Add(new wxStaticText(buttonPanel, wxID_ANY, ""));
		buttonSizer->Add(leftButton, 1, wxALL | wxALIGN_CENTER);
		buttonSizer->Add(new wxStaticText(buttonPanel, wxID_ANY, ""));
		buttonSizer->Add(rightButton, 1, wxALL | wxALIGN_CENTER);
		buttonSizer->Add(okButton, 1, wxALL | wxALIGN_CENTER);
		buttonSizer->Add(downButton, 1, wxALL | wxALIGN_CENTER);
		buttonSizer->Add(stopButton, 1, wxALL | wxALIGN_CENTER);

		this->queue = new CalxActionQueue(wxGetApp().getSystemManager(), this);
		this->queue->Run();
		int_conf_t interval = wxGetApp()
		                          .getSystemManager()
		                          ->getConfiguration()
		                          .getEntry(CalxConfiguration::UserInterface)
		                          ->getInt(CalxWatcherConfiguration::RenderInterval, 50);
		if (interval != -1) {
			this->timer = new CalxCoordAdjustRepaintTimer(this);
			this->timer->Start((int) interval);
		} else {
			this->timer = nullptr;
		}

		this->viewer->Bind(wxEVT_MOTION, &CalxCoordAdjustDialog::OnMouseMove, this);
		this->viewer->Bind(wxEVT_LEFT_DOWN, &CalxCoordAdjustDialog::OnMouseClick,
		                   this);
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxCoordAdjustDialog::OnExit, this);
		setEnabled(true);
		Layout();
		Fit();
	}

	void CalxCoordAdjustDialog::adjust(CalxAdjustDirection dir) {
		this->queue->addAction(std::make_unique<CalxCoordAdjustAction>(
		    this, this->handle, dir, this->stepSpin->GetValue(),
		    this->speedSpin->GetValue()));
	}

	void CalxCoordAdjustDialog::setEnabled(bool e) {
		for (auto i = buttonPanel->GetChildren().begin();
		     i != buttonPanel->GetChildren().end(); ++i) {
			if (*i != this->stopButton) {
				(*i)->Enable(e);
			}
		}
		this->prmsPanel->Enable(e);
		this->stopButton->Enable(!e);
	}

	void CalxCoordAdjustDialog::OnOkClick(wxCommandEvent &evt) {
		Close(true);
	}

	void CalxCoordAdjustDialog::OnStopClick(wxCommandEvent &evt) {
		this->queue->stopCurrent();
	}

	void CalxCoordAdjustDialog::OnLeftClick(wxCommandEvent &evt) {
		this->adjust(CalxAdjustDirection::Left);
	}

	void CalxCoordAdjustDialog::OnRightClick(wxCommandEvent &evt) {
		this->adjust(CalxAdjustDirection::Right);
	}

	void CalxCoordAdjustDialog::OnUpClick(wxCommandEvent &evt) {
		this->adjust(CalxAdjustDirection::Up);
	}

	void CalxCoordAdjustDialog::OnDownClick(wxCommandEvent &evt) {
		this->adjust(CalxAdjustDirection::Down);
	}

	void CalxCoordAdjustDialog::OnExit(wxCloseEvent &evt) {
		this->queue->stop();
		this->timer->Stop();
		Destroy();
	}

	void CalxCoordAdjustDialog::OnMouseMove(wxMouseEvent &evt) {
		if (evt.Leaving()) {
			this->mouseCoords->SetLabel("");
		}
		wxSize real_size = this->viewer->GetSize();
		coord_rect_t plane_size = this->handle->getFloatPlane()->getFloatSize();
		wxPoint mouse = this->viewer->ScreenToClient(wxGetMousePosition());
		if (mouse.x >= 0 && mouse.y >= 0 && mouse.x < real_size.x &&
		    mouse.y < real_size.y) {
			std::string units = wxGetApp().getUnitProcessor().getUnits();
			if (!units.empty()) {
				units = " " + units;
			}
			double rx = mouse.x * plane_size.w / real_size.x + plane_size.x;
			double ry =
			    plane_size.h - mouse.y * plane_size.h / real_size.y + plane_size.y;
			std::string res =
			    FORMAT(__("x: %s%s; y: %s%s"),
			           wxGetApp().getUnitProcessor().formatDouble(rx), units,
			           wxGetApp().getUnitProcessor().formatDouble(ry), units);
			this->mouseCoords->SetLabel(res);
		}
	}

	void CalxCoordAdjustDialog::OnMouseClick(wxMouseEvent &evt) {
		wxSize real_size = this->viewer->GetSize();
		coord_rect_t plane_size = this->handle->getFloatPlane()->getFloatSize();
		wxPoint mouse = this->viewer->ScreenToClient(wxGetMousePosition());
		if (mouse.x >= 0 && mouse.y >= 0 && mouse.x < real_size.x &&
		    mouse.y < real_size.y) {
			double rx = mouse.x * plane_size.w / real_size.x + plane_size.x;
			double ry =
			    plane_size.h - mouse.y * plane_size.h / real_size.y + plane_size.y;

			coord_point_t dest = { rx, ry };
			this->queue->addAction(std::make_unique<CalxCoordAdjustMoveAction>(
			    this, this->handle, dest, this->speedSpin->GetValue()));
		}
	}

	void CalxCoordAdjustDialog::updateUI() {
		this->viewer->Refresh();
		coord_point_t pos = this->handle->getFloatPlane()->getFloatPosition();
		this->position->SetLabel(FORMAT(
		    __("%sx%s %s"), wxGetApp().getUnitProcessor().formatDouble(pos.x),
		    wxGetApp().getUnitProcessor().formatDouble(pos.y),
		    wxGetApp().getUnitProcessor().getUnits()));
	}
}  // namespace CalXUI
