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

#include "ui/dev/CalxSerialMotor.h"
#include "ui/dev/CalxCOMSelectDialog.h"
#include "ui/dev/CalxDeviceConstructor.h"

namespace CalXUI {

	class CalxSerialMotorConnectAction : public CalxAction {
	 public:
		CalxSerialMotorConnectAction(CalxDevicePool *pool,
		                             DeviceConnectionPrms *prms) {
			this->pool = pool;
			this->prms = prms;
		}
		virtual ~CalxSerialMotorConnectAction() {
			delete this->prms;
		}
		virtual void perform(SystemManager *sysman) {
			std::shared_ptr<MotorController> ctrl = sysman->getMotorControllerSet().connectDevice(prms).lock();
			if (ctrl == nullptr) {
				wxMessageBox(__("Motor can't be connected"), __("Connection error"),
				             wxICON_WARNING);
			} else {
				pool->appendDevice(new CalxMotorConstructor(pool, ctrl));
			}
		}
		virtual void stop() {}

	 private:
		CalxDevicePool *pool;
		DeviceConnectionPrms *prms;
	};

	void CalxSerialMotorFactory::newDevice(wxWindow *win, CalxDevicePool *pool,
	                                       CalxActionQueue *queue) {
		CalxCOMSelectDialog *dialog = new CalxCOMSelectDialog(win, wxID_ANY);
		dialog->ShowModal();
		if (dialog->getPort() != -1) {
			DeviceSerialPortConnectionPrms *prms =
			    new DeviceSerialPortConnectionPrms();
			prms->port = (uint8_t) dialog->getPort();
			prms->speed = (uint32_t) dialog->getSpeed();
			prms->parity = dialog->getParity();
			queue->addAction(
			    std::make_unique<CalxSerialMotorConnectAction>(pool, prms));
		}
		dialog->Destroy();
	}
}  // namespace CalXUI
