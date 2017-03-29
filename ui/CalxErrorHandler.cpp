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


#include "CalxErrorHandler.h"

namespace CalXUI {
	
	CalxErrorHandler::CalxErrorHandler(SystemManager *sysman) {
		this->sysman = sysman;
	}
	
	CalxErrorHandler::~CalxErrorHandler() {
		
	}
	
	void CalxErrorHandler::handle(ErrorCode code) {
		if (code != ErrorCode::NoError) {
			wxMessageBox("System error(" + std::to_string(code) + ")", "System error", wxICON_ERROR);
		}
	}
}