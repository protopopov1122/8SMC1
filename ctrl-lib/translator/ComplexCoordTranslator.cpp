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

#include "ctrl-lib/translator/CoordTranslator.h"

namespace CalX {

	ComplexCoordTranslator::ComplexCoordTranslator(std::shared_ptr<CoordTranslator> root)
	    : CoordTranslator::CoordTranslator(CoordType::ComplexCoord) {
		this->list.push_back(root);
		INIT_LOG("ComplexCoordTranslator");
	}

	ComplexCoordTranslator::~ComplexCoordTranslator() {
		this->list.clear();
		DESTROY_LOG("ComplexCoordTranslator");
	}

	size_t ComplexCoordTranslator::getSize() {
		return this->list.size();
	}

	std::shared_ptr<CoordTranslator> ComplexCoordTranslator::getTranslator(size_t i) {
		if (i >= this->list.size()) {
			return nullptr;
		}
		return this->list.at(i);
	}

	void ComplexCoordTranslator::add(std::shared_ptr<CoordTranslator> t) {
		this->list.push_back(t);
	}

	bool ComplexCoordTranslator::remove(size_t i) {
		if (i >= this->list.size()) {
			return false;
		}
		this->list.erase(this->list.begin() + (std::ptrdiff_t) i);
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

	bool ComplexCoordTranslator::insert(size_t i, std::shared_ptr<CoordTranslator> t) {
		if (i >= this->list.size()) {
			return false;
		}
		this->list.insert(this->list.begin() + (std::ptrdiff_t) i, t);
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

	coord_point_t ComplexCoordTranslator::floatGet(double x, double y) {
		return this->list.at(this->list.size() - 1)->floatGet(x, y);
	}

	coord_point_t ComplexCoordTranslator::floatGet(coord_point_t pnt) {
		return this->list.at(this->list.size() - 1)->floatGet(pnt);
	}

	std::shared_ptr<CoordTranslator> ComplexCoordTranslator::getBase() {
		if (this->list.empty()) {
			return nullptr;
		}
		return this->list.at(0);
	}

	void ComplexCoordTranslator::setBase(std::shared_ptr<CoordTranslator> t) {
		remove(0);
		insert(0, t);
	}

	std::unique_ptr<CoordTranslator> ComplexCoordTranslator::clone(std::shared_ptr<CoordTranslator> base) {
		if (base == nullptr) {
			base = this->list.at(0)->clone(nullptr);
		}
		std::unique_ptr<ComplexCoordTranslator> trans = std::make_unique<ComplexCoordTranslator>(base);
		for (size_t i = 1; i < this->list.size(); i++) {
			trans->add(this->getTranslator(i)->clone(trans->getTranslator(i - 1)));
		}
		return trans;
	}
}
