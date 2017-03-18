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


#include "CoordTranslator.h"

namespace CalX {
	
	ComplexCoordTranslator::ComplexCoordTranslator(CoordTranslator *root)
		: CoordTranslator::CoordTranslator(CoordType::ComplexCoord) {
		this->list.push_back(root);	
	}
	
	ComplexCoordTranslator::~ComplexCoordTranslator() {
		for (const auto& t : this->list) {
			delete t;
		}
	}
	
	size_t ComplexCoordTranslator::getSize() {
		return this->list.size();
	}
	
	CoordTranslator *ComplexCoordTranslator::get(size_t i) {
		if (i >= this->list.size()) {
			return nullptr;
		}
		return this->list.at(i);
	}
	
	void ComplexCoordTranslator::add(CoordTranslator *t) {
		this->list.push_back(t);
	}
	
	bool ComplexCoordTranslator::remove(size_t i) {
		if (i >= this->list.size()) {
			return false;
		}
		CoordTranslator *trans = this->list.at(i);
		delete trans;
		this->list.erase(this->list.begin() + i);
		if (i >= this->list.size()) {
			return true;
		}
		if (i > 0) {
			this->list.at(i)->setBase(this->list.at(i - 1));
		} else {
			this->list.at(i)->setBase(nullptr);
		}
		return true;
	}
	
	bool ComplexCoordTranslator::insert(size_t i, CoordTranslator *t) {
		if (i >= this->list.size()) {
			return false;
		}
		this->list.insert(this->list.begin() + i, t);
		this->list.at(i + 1)->setBase(t);
		if (i > 0) {
			t->setBase(this->list.at(i - 1));
		} else {
			t->setBase(nullptr);
		}
		return true;
	}
	
	motor_point_t ComplexCoordTranslator::get(double x, double y) {
		return this->list.at(this->list.size() - 1)->get(x, y);
	}
	
	coord_point_t ComplexCoordTranslator::get(motor_point_t pnt) {
		return this->list.at(this->list.size() - 1)->get(pnt);
	}
	
	CoordTranslator *ComplexCoordTranslator::getBase() {
		if (this->list.empty()) {
			return nullptr;
		}
		return this->list.at(0);
	}
	
	void ComplexCoordTranslator::setBase(CoordTranslator *t) {
		remove(0);
		insert(0, t);
	}
}