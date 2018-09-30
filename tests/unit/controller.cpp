#include "catch.hpp"
#include "ctrl-lib/ctrl/MotorController.h"
#include "device/emulated/EmuDeviceManager.h"

using namespace CalX;

#define REQUIRE_NOERR(x) REQUIRE((x) == ErrorCode::NoError)

TEST_CASE("Motor controller state", "[controller]") {
  EmuDeviceManager devman;
  ConfigManager config;
  Motor *motor = devman.connectMotor(nullptr);
  MotorController controller(config, *motor);
  controller.enablePower(true);
  SECTION("Controller-motor proxies") {
    REQUIRE(&controller.getDevice() == motor);
    REQUIRE(&controller.getMotor() == motor);
    REQUIRE(controller.getID() == motor->getID());
    REQUIRE(&controller.getConfiguration() == &config);
    REQUIRE(controller.getPowerState() == motor->getPowerState());
    REQUIRE(controller.getPosition() == motor->getPosition());
    REQUIRE(controller.isMoving() == motor->isRunning());
  }
  SECTION("Controller power management") {
    REQUIRE(controller.getPowerState() != Power::NoPower);
    controller.flipPower();
    REQUIRE(controller.getPowerState() == Power::NoPower);
    controller.enablePower(true);
    controller.startMove(0, 0.0f);
    REQUIRE(controller.getPowerState() == Power::FullPower);
    controller.enablePower(false);
    REQUIRE(controller.getPowerState() == Power::NoPower);
  }
  SECTION("Controller synchronous movement") {
    const motor_coord_t POSITION = 10;
    controller.startMove(POSITION, 0.0f);
    REQUIRE(controller.getPosition() == POSITION);
    controller.startRelativeMove(POSITION, 0.0f);
    REQUIRE(controller.getPosition() == 2*POSITION);
    controller.startRelativeMove(-2*POSITION, 0.0f);
    REQUIRE(controller.getPosition() == 0);
  }
  SECTION("Controller trailer management") {
    REQUIRE_NOERR(controller.moveToTrailer(TrailerId::Trailer1));
    REQUIRE(controller.getPosition() == -EMU_MOTOR_SIZE / 2 + TRAILER_COMEBACK);
    REQUIRE_NOERR(controller.startMove(0, 0.0f));
    REQUIRE_NOERR(controller.moveToTrailer(TrailerId::Trailer2));
    REQUIRE(controller.getPosition() == EMU_MOTOR_SIZE / 2 - TRAILER_COMEBACK);
    REQUIRE_NOERR(controller.startMove(-EMU_MOTOR_SIZE, 0.0f));
    REQUIRE(controller.isTrailerPressed(TrailerId::Trailer1));
    REQUIRE_NOERR(controller.startMove(EMU_MOTOR_SIZE, 0.0f));
    REQUIRE(controller.isTrailerPressed(TrailerId::Trailer2));
    REQUIRE_NOERR(controller.startMove(0, 0.0f));
    REQUIRE(!(controller.isTrailerPressed(TrailerId::Trailer2) || controller.isTrailerPressed(TrailerId::Trailer2)));
    REQUIRE_NOERR(controller.checkTrailers());
  }
}