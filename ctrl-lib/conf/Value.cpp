#include "ctrl-lib/conf/ConfigManager.h"

namespace CalX {

	ConfigurationValue ConfigurationValue::Empty;

  ConfigurationValue::ConfigurationValue()
    : type(ConfigValueType::None) {}

  ConfigurationValue::ConfigurationValue(int_conf_t value)
    : type(ConfigValueType::Integer), value(value) {}

  ConfigurationValue::ConfigurationValue(real_conf_t value)
    : type(ConfigValueType::Real), value(value) {}

  ConfigurationValue::ConfigurationValue(bool value)
    : type(ConfigValueType::Boolean), value(value) {}

  ConfigurationValue::ConfigurationValue(std::string value)
    : type(ConfigValueType::String), value(std::move(value)) {}

  ConfigurationValue::ConfigurationValue(const ConfigurationValue &value)
    : type(value.type), value(value.value) {}

  ConfigValueType ConfigurationValue::getType() const {
    return this->type;
  }

  bool ConfigurationValue::is(ConfigValueType type) const {
    return this->type == type;
  }

  int_conf_t ConfigurationValue::getInt(int_conf_t defaultValue) const {
    if (this->is(ConfigValueType::Integer)) {
      return std::get<int_conf_t>(this->value);
    } else {
      return defaultValue;
    }
  }

  real_conf_t ConfigurationValue::getReal(real_conf_t defaultValue) const {
    if (this->is(ConfigValueType::Real)) {
      return std::get<real_conf_t>(this->value);
    } else {
      return defaultValue;
    }
  }

  bool ConfigurationValue::getBool(bool defaultValue) const {
    if (this->is(ConfigValueType::Boolean)) {
      return std::get<bool>(this->value);
    } else {
      return defaultValue;
    }
  }

  std::string ConfigurationValue::getString(std::string defaultValue) const {
    if (this->is(ConfigValueType::String)) {
      return std::get<std::string>(this->value);
    } else {
      return defaultValue;
    }
  }

  std::string ConfigurationValue::toString() const {
    switch (this->type) {
      case ConfigValueType::Integer:
        return std::to_string(this->getInt());
      case ConfigValueType::Real:
        return std::to_string(this->getReal());
      case ConfigValueType::Boolean:
        return this->getBool() ? "true" : "false";
      case ConfigValueType::String:
        return this->getString();
      case ConfigValueType::None:
        return "";
    }
    return "";
  }
}