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

#include "ctrl-lib/Resource.h"

namespace CalX {

	UsableResource::UsableResource() : counter(0) {}

	void UsableResource::use() {
		this->counter++;
	}

	void UsableResource::unuse() {
		if (this->counter == 0) {
			LOG_ERROR("UsableResource",
			          "Decrement of zero use counter. Must be an algorithm issue");
		} else {
			this->counter--;
		}
	}

	uint8_t UsableResource::getResourceCounter() const {
		return this->counter;
	}

	bool UsableResource::isResourceUsed() const {
		return this->counter != 0;
	}

	SessionableResource::SessionableResource() : session_state(false) {}

	ErrorCode SessionableResource::open_session() {
		if (this->session_state) {
			LOG_ERROR("SessionableResource",
			          "Opening already opened session. Must be an algorithm error");
		}
		this->session_state = true;
		return ErrorCode::NoError;
	}

	ErrorCode SessionableResource::close_session() {
		if (!this->session_state) {
			LOG_ERROR("SessionableResource",
			          "Closing already closed session. Must be an algorithm error");
		}
		this->session_state = false;
		return ErrorCode::NoError;
	}

	bool SessionableResource::isSessionOpened() const {
		return this->session_state;
	}

	ResourceUse::ResourceUse(UsableResource &res) : resource(&res) {
		this->resource->use();
	}

	ResourceUse::ResourceUse(UsableResource *res) : resource(res) {
		if (this->resource != nullptr) {
			this->resource->use();
		}
	}

	ResourceUse::~ResourceUse() {
		if (this->resource != nullptr) {
			this->resource->unuse();
		}
	}

	UsableResource *ResourceUse::swap(UsableResource &res) {
		return this->swap(&res);
	}

	UsableResource *ResourceUse::swap(UsableResource *res) {
		UsableResource *oldRes = this->resource;
		this->resource = res;
		if (this->resource != nullptr) {
			this->resource->use();
		}
		if (oldRes != nullptr) {
			oldRes->unuse();
		}
		return oldRes;
	}

	ResourceSession::ResourceSession(SessionableResource &res) : resource(&res) {
		this->errcode = this->resource->open_session();
	}

	ResourceSession::ResourceSession(SessionableResource *res) : resource(res) {
		if (this->resource) {
			this->errcode = this->resource->open_session();
		} else {
			this->errcode = ErrorCode::NoError;
		}
	}

	ResourceSession::~ResourceSession() {
		if (this->resource) {
			this->resource->close_session();
			this->resource = nullptr;
		}
	}

	ErrorCode ResourceSession::getStatus() const {
		return this->errcode;
	}

	ErrorCode ResourceSession::close() {
		if (this->resource != nullptr) {
			this->errcode = this->resource->close_session();
		}
		return this->errcode;
	}
}  // namespace CalX