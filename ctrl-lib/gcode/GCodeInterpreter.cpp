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

	static GCodeOperation getGCodeOperation(const gcl::GCodeSystemCommand &cmd) {
		if (cmd.getFunctionType() == gcl::GCodeSystemCommand::FunctionType::General && GCODE_OPERATIONS.count(cmd.getFunction().getInteger()) != 0) {
			return static_cast<GCodeOperation>(cmd.getFunction().getInteger());
		} else {
			return GCodeOperation::None;
		}
	}

	static double getRealConstant(const gcl::GCodeIRConstant &cnst) {
		if (cnst.is(gcl::GCodeIRConstant::Type::Integer)) {
			return static_cast<double>(cnst.getFloat());
		} else {
			return cnst.getFloat();
		}
	}

	GCodeInterpreter::GCodeInterpreter(CoordPlane &plane, std::shared_ptr<CoordTranslator> trans, ConfigurationCatalogue &config, float speed, TaskState &state)
		: plane(plane), trans(trans), config(config), speed(speed), state(state) {
		

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

	ErrorCode GCodeInterpreter::run(gcl::GCodeIRModule &module) {
		try {
			this->gcl::GCodeInterpreter::execute(module);
			return ErrorCode::NoError;
		} catch (ErrorCode errcode) {
			return errcode;
		} catch (...) {
			return ErrorCode::LowLevelError;
		}
	}

	void GCodeInterpreter::execute(const gcl::GCodeSystemCommand &cmd) {
		if (!this->state.work) {
			throw ErrorCode::NoError;
		}
		const int_conf_t CHORD_COUNT =
		    config.getEntry(CalxConfiguration::Core)->getInt("chord_count", 100);
		ErrorCode errcode = ErrorCode::NoError;
		motor_point_t rel_offset = { 0, 0 };
		if (relative_pos) {
			rel_offset = plane.getPosition();
		}
		switch (getGCodeOperation(cmd)) {
			case GCodeOperation::RapidMove: {
				coord_point_t dest = translator->get(plane.getPosition());
				if (cmd.hasParameter('X')) {
					gcl::GCodeIRConstant prm = cmd.getParameter('X');
					dest.x = getRealConstant(prm);
				}
				if (cmd.hasParameter('Y')) {
					gcl::GCodeIRConstant prm = cmd.getParameter('Y');
					dest.y = getRealConstant(prm);
				}
				motor_point_t mdest = translator->get(dest.x, dest.y);
				mdest.x += rel_offset.x;
				mdest.y += rel_offset.y;
				errcode = plane.move(mdest, speed, false);
			} break;
			case GCodeOperation::LinearMove: {
				coord_point_t dest = translator->get(plane.getPosition());
				if (cmd.hasParameter('X')) {
					gcl::GCodeIRConstant prm = cmd.getParameter('X');
					dest.x = getRealConstant(prm);
				}
				if (cmd.hasParameter('Y')) {
					gcl::GCodeIRConstant prm = cmd.getParameter('Y');
					dest.y = getRealConstant(prm);
				}
				motor_point_t mdest = translator->get(dest.x, dest.y);
				mdest.x += rel_offset.x;
				mdest.y += rel_offset.y;
				errcode = plane.move(mdest, speed, true);
			} break;
			case GCodeOperation::ClockwiseArc:
				if (cmd.hasParameter('I') || cmd.hasParameter('J')) {
					coord_point_t dest = translator->get(plane.getPosition());
					coord_point_t cen = { 0, 0 };
					if (cmd.hasParameter('X')) {
						gcl::GCodeIRConstant prm = cmd.getParameter('X');
						dest.x = getRealConstant(prm);
					}
					if (cmd.hasParameter('Y')) {
						gcl::GCodeIRConstant prm = cmd.getParameter('Y');
						dest.y = getRealConstant(prm);
					}
					if (cmd.hasParameter('I')) {
						gcl::GCodeIRConstant prm = cmd.getParameter('I');
						cen.x = getRealConstant(prm);
					}
					if (cmd.hasParameter('J')) {
						gcl::GCodeIRConstant prm = cmd.getParameter('J');
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
				if (cmd.hasParameter('I') || cmd.hasParameter('J')) {
					coord_point_t dest = translator->get(plane.getPosition());
					coord_point_t cen = { 0, 0 };
					if (cmd.hasParameter('X')) {
						gcl::GCodeIRConstant prm = cmd.getParameter('X');
						dest.x = getRealConstant(prm);
					}
					if (cmd.hasParameter('Y')) {
						gcl::GCodeIRConstant prm = cmd.getParameter('Y');
						dest.y = getRealConstant(prm);
					}
					if (cmd.hasParameter('I')) {
						gcl::GCodeIRConstant prm = cmd.getParameter('I');
						cen.x = getRealConstant(prm);
					}
					if (cmd.hasParameter('J')) {
						gcl::GCodeIRConstant prm = cmd.getParameter('J');
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
				if (cmd.hasParameter('X')) {
					double newx = getRealConstant(cmd.getParameter('X'));
					offset.x = curpos.x - newx;
				}
				if (cmd.hasParameter('Y')) {
					double newy = getRealConstant(cmd.getParameter('Y'));
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
}  // namespace CalX
