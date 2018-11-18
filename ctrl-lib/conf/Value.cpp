#include "ctrl-lib/conf/ConfigManager.h"

namespace CalX {

	ConfigurationValue ConfigurationValue::Empty;

  ConfigurationValue::ConfigurationValue()
    : type(ConfigurationValueType::None) {}

  ConfigurationValue::ConfigurationValue(int_conf_t value)
    : type(ConfigurationValueType::Integer), value(value) {}

  ConfigurationValue::ConfigurationValue(real_conf_t value)
    : type(ConfigurationValueType::Real), value(value) {}

  ConfigurationValue::ConfigurationValue(bool value)
    : type(ConfigurationValueType::Boolean), value(value) {}

  ConfigurationValue::ConfigurationValue(std::string value)
    : type(ConfigurationValueType::String), value(std::move(value)) {}

  ConfigurationValue::ConfigurationValue(const ConfigurationValue &value)
    : type(value.type), value(value.value) {}

  ConfigurationValueType ConfigurationValue::getType() const {
    return this->type;
  }

  bool ConfigurationValue::is(ConfigurationValueType type) const {
    return this->type == type;
  }

  int_conf_t ConfigurationValue::getInt(int_conf_t defaultValue) const {
    if (this->is(ConfigurationValueType::Integer)) {
      return std::get<int_conf_t>(this->value);
    } else {
      return defaultValue;
    }
  }

  real_conf_t ConfigurationValue::getReal(real_conf_t defaultValue) const {
    if (this->is(ConfigurationValueType::Real)) {
      return std::get<real_conf_t>(this->value);
    } else {
      return defaultValue;
    }
  }

  bool ConfigurationValue::getBool(bool defaultValue) const {
    if (this->is(ConfigurationValueType::Boolean)) {
      return std::get<bool>(this->value);
    } else {
      return defaultValue;
    }
  }

  std::string ConfigurationValue::getString(std::string defaultValue) const {
    if (this->is(ConfigurationValueType::String)) {
      return std::get<std::string>(this->value);
    } else {
      return defaultValue;
    }
  }

  std::string ConfigurationValue::toString() const {
    switch (this->type) {
      case ConfigurationValueType::Integer:
        return std::to_string(this->getInt());
      case ConfigurationValueType::Real:
        return std::to_string(this->getReal());
      case ConfigurationValueType::Boolean:
        return this->getBool() ? "true" : "false";
      case ConfigurationValueType::String:
        return this->getString();
      case ConfigurationValueType::None:
        return "";
    }
    return "";
  }

	bool ConfiguationFlatDictionary::is(const std::string &id, ConfigurationValueType type) const {
    return this->has(id) && this->get(id).is(type);
	}

	int_conf_t ConfiguationFlatDictionary::getInt(const std::string &key, int_conf_t def) const {
		return this->get(key).getInt(def);
	}

	real_conf_t ConfiguationFlatDictionary::getReal(const std::string &key, real_conf_t def) const {
		return this->get(key).getReal(def);
	}

	bool ConfiguationFlatDictionary::getBool(const std::string &key, bool def) const {
		return this->get(key).getBool(def);
	}

	std::string ConfiguationFlatDictionary::getString(const std::string &key, const std::string &def) const {
		return this->get(key).getString(def);
	}
}