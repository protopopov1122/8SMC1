#include "CoordTranslator.h"

namespace _8SMC1 {
	
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
		}
		return true;
	}
	
	motor_point_t ComplexCoordTranslator::get(long double x, long double y) {
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