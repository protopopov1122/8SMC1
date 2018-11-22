#include "catch.hpp"
#include "ctrl-lib/plane/CoordPlane.h"
#include "device/emulated/EmuDeviceManager.h"
#include "ctrl-lib/conf/ConfigManager.h"

using namespace CalX;

#define REQUIRE_NOERR(x) REQUIRE((x) == ErrorCode::NoError)

TEST_CASE("Coord controller") {
  ConfigManager config;
  EmuDeviceManager devman;
  std::shared_ptr<MotorController> motor1 = std::make_shared<MotorController>(config, *devman.connectMotor(nullptr));
  std::shared_ptr<MotorController> motor2 = std::make_shared<MotorController>(config, *devman.connectMotor(nullptr));
  std::shared_ptr<InstrumentController> instrument = std::make_shared<InstrumentController>(config, *devman.connectInstrument(nullptr));
  motor1->enablePower(true);
  motor2->enablePower(true);
  CoordController plane(config, motor1, motor2, instrument);

  SECTION("Controller parameters") {
    REQUIRE(plane.getXAxis() == motor1);
    REQUIRE(plane.getYAxis() == motor2);
    REQUIRE(plane.getInstrument() == instrument);
    REQUIRE(plane.getStatus() == CoordPlaneStatus::Idle);
  }
  SECTION("Controller movement") {
    const motor_coord_t X = 10;
    const motor_coord_t Y = 20;
    REQUIRE_NOERR(plane.move(motor_point_t(X, Y), 0.0f, true));
    REQUIRE(motor1->getPosition() == X);
    REQUIRE(motor2->getPosition() == Y);
    REQUIRE(plane.getPosition().x == X);
    REQUIRE(plane.getPosition().y == Y);
  }
  SECTION("Controller calibration") {
    REQUIRE_NOERR(plane.calibrate(TrailerId::Trailer1));
    REQUIRE(plane.getPosition().x == -EMU_MOTOR_SIZE / 2 + TRAILER_COMEBACK);
    REQUIRE(plane.getPosition().y == -EMU_MOTOR_SIZE / 2 + TRAILER_COMEBACK);
    REQUIRE_NOERR(plane.move(motor_point_t(0, 0), 0.0f, true));
    REQUIRE_NOERR(plane.calibrate(TrailerId::Trailer2));
    REQUIRE(plane.getPosition().x == EMU_MOTOR_SIZE / 2 - TRAILER_COMEBACK);
    REQUIRE(plane.getPosition().y == EMU_MOTOR_SIZE / 2 - TRAILER_COMEBACK);
  }
  SECTION("Controller measurement") {
    REQUIRE(!plane.isMeasured());
    REQUIRE(plane.getSize().w == 0);
    REQUIRE(plane.getSize().h == 0);
    REQUIRE_NOERR(plane.measure(TrailerId::Trailer1));
    REQUIRE(plane.isMeasured());
    REQUIRE(plane.getSize().w == EMU_MOTOR_SIZE - TRAILER_COMEBACK);
    REQUIRE(plane.getSize().h == EMU_MOTOR_SIZE - TRAILER_COMEBACK);
    REQUIRE_NOERR(plane.measure(TrailerId::Trailer2));
    REQUIRE(plane.isMeasured());
    REQUIRE(plane.getSize().w == EMU_MOTOR_SIZE - TRAILER_COMEBACK);
    REQUIRE(plane.getSize().h == EMU_MOTOR_SIZE - TRAILER_COMEBACK);
  }
  SECTION("Controller sessions") {
    REQUIRE(!plane.isSessionOpened());
    REQUIRE_NOERR(plane.open_session());
    REQUIRE(plane.isSessionOpened());
    REQUIRE_NOERR(plane.open_session());
    REQUIRE_NOERR(plane.open_session());
    REQUIRE(plane.isSessionOpened());
    REQUIRE_NOERR(plane.close_session());
    REQUIRE(!plane.isSessionOpened());
    REQUIRE_NOERR(plane.close_session());
    REQUIRE_NOERR(plane.close_session());
    REQUIRE(!plane.isSessionOpened());
  }
}