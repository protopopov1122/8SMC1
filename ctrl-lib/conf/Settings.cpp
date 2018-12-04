#include "ctrl-lib/conf/Settings.h"

namespace CalX {

  SettingsConfigurationEntry::SettingsConfigurationEntry(ConfigurationCatalogue *master, ConfigurationCatalogue &fallback,
    const std::string &entryName, bool createEntry)
    : master(nullptr), fallback(nullptr) {
    if (master) {
      this->master = master->getEntry(entryName, createEntry);
    }
    this->fallback = fallback.getEntry(entryName, false);
  }

  const ConfigurationValue &SettingsConfigurationEntry::get(const std::string &key) const {
    if (this->master && this->master->has(key)) {
      return this->master->get(key);
    } else if (this->fallback && this->fallback->has(key)) {
      return this->fallback->get(key);
    } else {
      return ConfigurationValue::Empty;
    }
  }

  bool SettingsConfigurationEntry::has(const std::string &key) const {
    return (this->master && this->master->has(key)) ||
      (this->fallback && this->fallback->has(key));
  }

  bool SettingsConfigurationEntry::put(const std::string &key, const ConfigurationValue &value) {
    if (this->master) {
      return this->master->put(key, value);
    } else {
      return false;
    }
  }

  bool SettingsConfigurationEntry::remove(const std::string &key) {
    if (this->master) {
      return this->master->remove(key);
    } else {
      return false;
    }
  }

  void SettingsConfigurationEntry::visit(std::function<void (const std::string &, const ConfigurationValue &)> visitor) const {
    if (this->master) {
      this->master->visit(visitor);
    }
  }

  SettingsConfiguration::SettingsConfiguration(SettingsRepository *settings, ConfigurationCatalogue &fallback)
    : master(settings != nullptr ? &settings->getSettings() : nullptr), fallback(fallback) {}

  SettingsConfiguration::SettingsConfiguration(ConfigurationCatalogue *master, ConfigurationCatalogue &fallback)
    : master(master), fallback(fallback) {}
  
  ConfiguationFlatDictionary *SettingsConfiguration::getEntry(const std::string &entryName, bool createNew) {
    if (this->entries.count(entryName)) {
      return this->entries[entryName].get();
    } else if (this->hasEntry(entryName) || createNew) {
      this->entries[entryName] = std::make_unique<SettingsConfigurationEntry>(this->master, this->fallback, entryName, createNew);
      return this->entries[entryName].get();
    } else {
      return nullptr;
    }
  }

  bool SettingsConfiguration::hasEntry(const std::string &entryName) const {
    return (this->master && this->master->hasEntry(entryName)) || this->fallback.hasEntry(entryName);
  }

  bool SettingsConfiguration::removeEntry(const std::string &entryName) {
    if (this->entries.count(entryName)) {
      this->entries.erase(entryName);
    }
    if (this->master) {
      return this->master->removeEntry(entryName);
    } else {
      return false;
    }
  }

  void SettingsConfiguration::visit(std::function<void (const std::string &, ConfiguationFlatDictionary &)> visitor) const {
    if (this->master) {
      this->master->visit(visitor);
    }
  }
}