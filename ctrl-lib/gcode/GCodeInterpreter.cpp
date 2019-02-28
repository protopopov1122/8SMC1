/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

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

#include "calx/ctrl-lib/gcode/GCodeInterpreter.h"
#include <iostream>

namespace CalX {

	static const std::set<int16_t> GCODE_OPERATIONS = {
		0,   // Rapid move
		1,   // Linear move
		2,   // Clockwise arc
		3,   // Counter-clockwise arc
		20,  // Switch to inches
		21,  // Switch to millimeters
		28,  // Go home
		90,  // Absolute positioning
		91,  // Relative positioning
		92   // Set position
	};

	static GCodeOperation getGCodeOperation(const gclr::GCodeRuntimeValue &function) {
		if (GCODE_OPERATIONS.count(function.getInteger()) != 0) {
			return static_cast<GCodeOperation>(function.getInteger());
		} else {
			return GCodeOperation::None;
		}
	}

	static double getRealConstant(const gclr::GCodeRuntimeValue &cnst) {
		return cnst.asFloat();
	}

	GCodeInterpreter::GCodeInterpreter(gclr::GCodeIRModule &module, CoordPlane &plane, std::shared_ptr<CoordTranslator> trans, ConfigurationCatalogue &config, float speed, TaskState &state)
		: gclr::GCodeInterpreter::GCodeInterpreter(module), plane(plane), trans(trans), config(config), speed(speed), state(state) {
		

		this->unit_trans =
		    std::make_shared<LinearCoordTranslator>(coord_point_t(0, 0), coord_scale_t(1, 1), trans);
		this->translator =
		    std::make_shared<LinearCoordTranslator>(coord_point_t(0, 0), coord_scale_t(1, 1), unit_trans);

		this->offset = translator->get(0, 0);
		this->invert = 1;
		if (translator->get(1, 0).x < offset.x) {
			this->invert *= -1;
		}
		if (translator->get(0, 1).y < offset.y) {
			this->invert *= -1;
		}
		this->relative_pos = false;
	}

	ErrorCode GCodeInterpreter::run() {
		try {
			this->gclr::GCodeInterpreter::execute();
			return ErrorCode::NoError;
		} catch (ErrorCode errcode) {
			return errcode;
		}
	}

	void GCodeInterpreter::syscall(gclr::GCodeSyscallType type, const gclr::GCodeRuntimeValue &function, const gclr::GCodeScopedDictionary<unsigned char> &args) {
		if (!this->state.work) {
			throw ErrorCode::NoError;
		}
		if (type != gclr::GCodeSyscallType::General) {
			return;
		}
		const int_conf_t CHORD_COUNT =
		    config.getEntry(CalxConfiguration::Core)->getInt("chord_count", 100);
		ErrorCode errcode = ErrorCode::NoError;
		motor_point_t rel_offset = { 0, 0 };
		if (relative_pos) {
			rel_offset = plane.getPosition();
		}
		switch (getGCodeOperation(function)) {
			case GCodeOperation::RapidMove: {
				coord_point_t dest = translator->get(plane.getPosition());
				if (args.has('X')) {
					gclr::GCodeRuntimeValue prm = args.get('X');
					dest.x = getRealConstant(prm);
				}
				if (args.has('Y')) {
					gclr::GCodeRuntimeValue prm = args.get('Y');
					dest.y = getRealConstant(prm);
				}
				motor_point_t mdest = translator->get(dest.x, dest.y);
				mdest.x += rel_offset.x;
				mdest.y += rel_offset.y;
				errcode = plane.move(mdest, speed, false);
			} break;
			case GCodeOperation::LinearMove: {
				coord_point_t dest = translator->get(plane.getPosition());
				if (args.has('X')) {
					gclr::GCodeRuntimeValue prm = args.get('X');
					dest.x = getRealConstant(prm);
				}
				if (args.has('Y')) {
					gclr::GCodeRuntimeValue prm = args.get('Y');
					dest.y = getRealConstant(prm);
				}
				motor_point_t mdest = translator->get(dest.x, dest.y);
				mdest.x += rel_offset.x;
				mdest.y += rel_offset.y;
				errcode = plane.move(mdest, speed, true);
			} break;
			case GCodeOperation::ClockwiseArc:
				if (args.has('I') || args.has('J')) {
					coord_point_t dest = translator->get(plane.getPosition());
					coord_point_t cen = { 0, 0 };
					if (args.has('X')) {
						gclr::GCodeRuntimeValue prm = args.get('X');
						dest.x = getRealConstant(prm);
					}
					if (args.has('Y')) {
						gclr::GCodeRuntimeValue prm = args.get('Y');
						dest.y = getRealConstant(prm);
					}
					if (args.has('I')) {
						gclr::GCodeRuntimeValue prm = args.get('I');
						cen.x = getRealConstant(prm);
					}
					if (args.has('J')) {
						gclr::GCodeRuntimeValue prm = args.get('J');
						cen.y = getRealConstant(prm);
					}
					motor_point_t current = plane.getPosition();
					motor_point_t mdest = translator->get(dest.x, dest.y);
					motor_point_t mcen = translator->get(cen.x, cen.y);
					mcen.x -= offset.x;
					mcen.y -= offset.y;
					mcen.x += current.x;
					mcen.y += current.y;
					mdest.x += rel_offset.x;
					mdest.y += rel_offset.y;
					errcode = plane.arc(mdest, mcen, CHORD_COUNT, speed, invert == 1);
				}
				break;
			case GCodeOperation::CounterClockwiseArc:
				if (args.has('I') || args.has('J')) {
					coord_point_t dest = translator->get(plane.getPosition());
					coord_point_t cen = { 0, 0 };
					if (args.has('X')) {
						gclr::GCodeRuntimeValue prm = args.get('X');
						dest.x = getRealConstant(prm);
					}
					if (args.has('Y')) {
						gclr::GCodeRuntimeValue prm = args.get('Y');
						dest.y = getRealConstant(prm);
					}
					if (args.has('I')) {
						gclr::GCodeRuntimeValue prm = args.get('I');
						cen.x = getRealConstant(prm);
					}
					if (args.has('J')) {
						gclr::GCodeRuntimeValue prm = args.get('J');
						cen.y = getRealConstant(prm);
					}
					motor_point_t current = plane.getPosition();
					motor_point_t mdest = translator->get(dest.x, dest.y);
					motor_point_t mcen = translator->get(cen.x, cen.y);
					mcen.x -= offset.x;
					mcen.y -= offset.y;
					mcen.x += current.x;
					mcen.y += current.y;
					mdest.x += rel_offset.x;
					mdest.y += rel_offset.y;
					errcode = plane.arc(mdest, mcen, CHORD_COUNT, speed, invert != 1);
				}
				break;
			case GCodeOperation::SwitchInches: {
				coord_scale_t scale = { 25.4, 25.4 };
				unit_trans->setScale(scale);
			} break;
			case GCodeOperation::SwitchMillimeters: {
				coord_scale_t scale = { 1.0, 1.0 };
				unit_trans->setScale(scale);
			} break;
			case GCodeOperation::Home: {
				coord_point_t dest = { 0, 0 };
				errcode = plane.move(translator->get(dest.x, dest.y), speed, false);
			} break;
			case GCodeOperation::AbsolutePositioning: {
				relative_pos = false;
			} break;
			case GCodeOperation::RelativePositioning: {
				relative_pos = true;
			} break;
			case GCodeOperation::SetPosition: {
				coord_point_t curpos = unit_trans->get(plane.getPosition());
				coord_point_t offset = translator->getOffset();
				if (args.has('X')) {
					double newx = getRealConstant(args.get('X'));
					offset.x = curpos.x - newx;
				}
				if (args.has('Y')) {
					double newy = getRealConstant(args.get('Y'));
					offset.y = curpos.y - newy;
				}
				translator->setOffset(offset);
			} break;
			default:
				break;
		}
		
		if (errcode != ErrorCode::NoError) {
			throw errcode;
		}
	}

	gclr::GCodeVariableScope &GCodeInterpreter::getSystemScope() {
		return this->systemScope;
	}
}  // namespace CalX
