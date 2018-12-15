#include "catch.hpp"
#include "ctrl-lib/ctrl/MotorController.h"
#include "ctrl-lib/ctrl/InstrumentController.h"
#include "device/emulated/EmuDeviceManager.h"
#include "ctrl-lib/conf/ConfigManager.h"

using namespace CalX;

#define REQUIRE_NOERR(x) REQUIRE((x) == ErrorCode::NoError)

TEST_CASE("Motor controller", "[controller]") {
	EmuDeviceManager devman;
	ConfigManager config;
	Motor *motor = devman.connectMotor(nullptr);
	REQUIRE(motor != nullptr);
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
		REQUIRE(controller.getPosition() == 2 * POSITION);
		controller.startRelativeMove(-2 * POSITION, 0.0f);
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
		REQUIRE(!(controller.isTrailerPressed(TrailerId::Trailer2) ||
		          controller.isTrailerPressed(TrailerId::Trailer2)));
		REQUIRE_NOERR(controller.checkTrailers());
	}
	SECTION("Motor events") {
		// TODO Add motor event tests
	}
}

TEST_CASE("Instrument controller", "[controller]") {
	EmuDeviceManager devman;
	ConfigManager config;
	Instrument *instrument = devman.connectInstrument(nullptr);
	REQUIRE(instrument != nullptr);
	InstrumentController controller(config, *instrument);
	SECTION("Instrument parameters") {
		REQUIRE(&controller.getDevice() == instrument);
		REQUIRE(&controller.getInstrument() == instrument);
		REQUIRE(controller.getID() == instrument->getID());
		REQUIRE(controller.isEnabled() == instrument->enabled());
		REQUIRE(controller.getMode() == instrument->getWorkingMode());
		REQUIRE(controller.getInfo().compare(instrument->getDeviceInfo()) == 0);
	}
	SECTION("Instrument state management") {
		REQUIRE(!controller.isEnabled());
		REQUIRE_NOERR(controller.enable(true));
		REQUIRE((controller.isEnabled() && instrument->enabled()));
		REQUIRE_NOERR(controller.enable(true));
		REQUIRE(controller.isEnabled());
		REQUIRE_NOERR(controller.flipState());
		REQUIRE((!controller.isEnabled() && !instrument->enabled()));
		REQUIRE_NOERR(controller.flipState());
		REQUIRE(controller.isEnabled());
		REQUIRE_NOERR(controller.enable(false));
		REQUIRE((!controller.isEnabled() && !instrument->enabled()));
	}
	SECTION("Instrument session management") {
		REQUIRE(!controller.isSessionOpened());
		REQUIRE_NOERR(controller.open_session());
		REQUIRE(controller.isSessionOpened());
		REQUIRE_NOERR(controller.open_session());
		REQUIRE_NOERR(controller.open_session());
		REQUIRE(controller.isSessionOpened());
		REQUIRE_NOERR(controller.close_session());
		REQUIRE(!controller.isSessionOpened());
		REQUIRE_NOERR(controller.close_session());
		REQUIRE_NOERR(controller.close_session());
		REQUIRE(!controller.isSessionOpened());
	}
	SECTION("Instrument states & sessions") {
		REQUIRE((!controller.isSessionOpened() && !controller.isEnabled()));
		REQUIRE_NOERR(controller.open_session());
		REQUIRE((controller.isSessionOpened() && !controller.isEnabled()));
		REQUIRE_NOERR(controller.close_session());
		REQUIRE((!controller.isSessionOpened() && !controller.isEnabled()));
		REQUIRE_NOERR(controller.enable(true));
		REQUIRE((controller.isSessionOpened() && controller.isEnabled()));
		REQUIRE_NOERR(controller.enable(false));
		REQUIRE((controller.isSessionOpened() && !controller.isEnabled()));
		REQUIRE_NOERR(controller.flipState());
		REQUIRE_NOERR(controller.close_session());
		REQUIRE((!controller.isSessionOpened() && !controller.isEnabled()));
	}
	SECTION("Instrument modes") {
		REQUIRE(controller.getMode() == InstrumentMode::Off);
		REQUIRE(controller.setMode(InstrumentMode::Prepare));
		REQUIRE(controller.getMode() == InstrumentMode::Prepare);
		REQUIRE(controller.setMode(InstrumentMode::Full));
		REQUIRE(controller.getMode() == InstrumentMode::Full);
		REQUIRE(controller.setMode(InstrumentMode::Off));
		REQUIRE(controller.getMode() == InstrumentMode::Off);
	}
	SECTION("Instrument events") {
		// TODO Add instrument event tests
	}
}