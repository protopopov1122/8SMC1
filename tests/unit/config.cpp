#include "catch.hpp"
#include "ctrl-lib/conf/ConfigManager.h"

using namespace CalX;

TEST_CASE("Configuration manager") {
  ConfigManager config;
  SECTION ("Entry management") {
    std::vector<std::shared_ptr<ConfigEntry>> entries;
    const std::string ENTRY = "test_entry";
    REQUIRE(!config.hasEntry(ENTRY));
    config.getEntries(entries);
    REQUIRE(entries.size() == 0);
    REQUIRE(!config.removeEntry(ENTRY));
    REQUIRE(config.getEntry(ENTRY, false) == nullptr);

    std::shared_ptr<ConfigEntry> entry = config.getEntry(ENTRY);
    REQUIRE(entry != nullptr);
    REQUIRE(config.hasEntry(ENTRY));
    REQUIRE(entry.get() == config.getEntry(ENTRY).get());
    for (int i = 0; i < 10; i++) {
      config.getEntry(ENTRY);
    }
    config.getEntries(entries);
    REQUIRE(entries.size() == 1);

    REQUIRE(config.removeEntry(ENTRY));
    REQUIRE(!config.hasEntry(ENTRY));
    entries.clear();
    config.getEntries(entries);
    REQUIRE(entries.size() == 0);

    const int ENTRY_COUNT = 10;
    for (int i = 0; i < ENTRY_COUNT; i++) {
      REQUIRE(!config.removeEntry(ENTRY + std::to_string(i)));
      config.getEntry(ENTRY + std::to_string(i));
    }
    entries.clear();
    config.getEntries(entries);
    REQUIRE(entries.size() == ENTRY_COUNT);
    for (int i = 0; i < ENTRY_COUNT; i++) {
      REQUIRE(config.hasEntry(ENTRY + std::to_string(i)));
      REQUIRE(config.removeEntry(ENTRY + std::to_string(i)));
    }
  }
}