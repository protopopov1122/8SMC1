#include "catch.hpp"
#include "ctrl-lib/translator/CoordTranslator.h"

using namespace CalX;

TEST_CASE("Basic coord translator getters & setters work", "[translator]") {
	BasicCoordTranslator translator(motor_point_t(0, 0), motor_size_t(1, 1));
	SECTION("Getting initial values") {
		REQUIRE(translator.getCenter().x == 0);
		REQUIRE(translator.getCenter().y == 0);
		REQUIRE(translator.getScale().w == 1);
		REQUIRE(translator.getScale().h == 1);
	}
	SECTION("Setting new center") {
		const motor_coord_t CENTER_X = 10;
		const motor_coord_t CENTER_Y = -10;
		translator.setCenter(motor_point_t(CENTER_X, CENTER_Y));
		REQUIRE(translator.getCenter().x == CENTER_X);
		REQUIRE(translator.getCenter().y == CENTER_Y);
	}
	SECTION("Setting new scale") {
		const motor_coord_t SCALE_W = 2;
		const motor_coord_t SCALE_H = 3;
		translator.setScale(motor_size_t(SCALE_W, SCALE_H));
		REQUIRE(translator.getScale().w == SCALE_W);
		REQUIRE(translator.getScale().h == SCALE_H);
	}
}

TEST_CASE("Basic coord translator coordinates <-> motor position",
          "[translator]") {
	const motor_coord_t CENTER_X = 10;
	const motor_coord_t CENTER_Y = 5;
	const motor_coord_t SCALE_W = 2;
	const motor_coord_t SCALE_H = 3;
	BasicCoordTranslator translator(motor_point_t(CENTER_X, CENTER_Y),
	                                motor_size_t(SCALE_W, SCALE_H));
	SECTION("Coordinates to motor position") {
		const double COORD_X = 5;
		const double COORD_Y = -10;
		motor_point_t position = translator.get(COORD_X, COORD_Y);
		REQUIRE(position.x == COORD_X * SCALE_W + CENTER_X);
		REQUIRE(position.y == COORD_Y * SCALE_H + CENTER_Y);
	}
	SECTION("Motor position to coordinates") {
		const motor_point_t position(4, 7);
		coord_point_t coordinates = translator.get(position);
		REQUIRE(coordinates.x ==
		        static_cast<double>(position.x - CENTER_X) / SCALE_W);
		REQUIRE(coordinates.y ==
		        static_cast<double>(position.y - CENTER_Y) / SCALE_H);
	}
}

TEST_CASE("Basic coord translator coordinates <-> floating point position",
          "[translator]") {
	const motor_coord_t CENTER_X = 10;
	const motor_coord_t CENTER_Y = 5;
	const motor_coord_t SCALE_W = 2;
	const motor_coord_t SCALE_H = 3;
	BasicCoordTranslator translator(motor_point_t(CENTER_X, CENTER_Y),
	                                motor_size_t(SCALE_W, SCALE_H));
	SECTION("Coordinates to floating point position") {
		const double COORD_X = 5;
		const double COORD_Y = -10;
		coord_point_t position = translator.floatGet(COORD_X, COORD_Y);
		REQUIRE(position.x == COORD_X * SCALE_W + CENTER_X);
		REQUIRE(position.y == COORD_Y * SCALE_H + CENTER_Y);
	}
	SECTION("Floating point position to coordinates") {
		const coord_point_t position(4, 7);
		coord_point_t coordinates = translator.floatGet(position);
		REQUIRE(coordinates.x == (position.x - CENTER_X) / SCALE_W);
		REQUIRE(coordinates.y == (position.y - CENTER_Y) / SCALE_H);
	}
}

TEST_CASE("Linear coord translator without base", "[translator]") {
	LinearCoordTranslator translator(coord_point_t(0, 0), coord_scale_t(1, 1));
	SECTION("Translation with no changes") {
		const double X = 5;
		const double Y = -5;
		motor_point_t position = translator.get(X, Y);
		coord_point_t backwards = translator.get(position);
		coord_point_t float_position = translator.floatGet(X, Y);
		coord_point_t backwards_float_position =
		    translator.floatGet(float_position);
		REQUIRE(position.x == X);
		REQUIRE(position.y == Y);
		REQUIRE(backwards.x == position.x);
		REQUIRE(backwards.y == position.y);
		REQUIRE(float_position.x == X);
		REQUIRE(float_position.y == Y);
		REQUIRE(backwards_float_position.x == float_position.x);
		REQUIRE(backwards_float_position.y == float_position.y);
	}
	SECTION("Getters & setters") {
		REQUIRE(translator.getOffset().x == 0);
		REQUIRE(translator.getOffset().y == 0);
		REQUIRE(translator.getScale().x == 1);
		REQUIRE(translator.getScale().y == 1);
		const double X = 4;
		const double Y = 2;
		const double W = 3;
		const double H = 0.5;
		translator.setOffset(coord_point_t(X, Y));
		translator.setScale(coord_scale_t(W, H));
		REQUIRE(translator.getOffset().x == X);
		REQUIRE(translator.getOffset().y == Y);
		REQUIRE(translator.getScale().x == W);
		REQUIRE(translator.getScale().y == H);
	}
}

TEST_CASE("Linear coord translator with base", "[translator]") {
	std::shared_ptr<BasicCoordTranslator> basic =
	    std::make_shared<BasicCoordTranslator>(motor_point_t(1, 2),
	                                           motor_size_t(3, 3));
	const double X = -3;
	const double Y = 6;
	const double W = 5;
	const double H = 0.2;
	LinearCoordTranslator translator(coord_point_t(X, Y), coord_scale_t(W, H),
	                                 basic);
	SECTION("Coordinate <-> motor point translation") {
		const double x = 2;
		const double y = -2;
		motor_point_t position = translator.get(x, y);
		REQUIRE(position.x == basic->get(x * W + X, y).x);
		REQUIRE(position.y == basic->get(x, y * H + Y).y);
	}
}
