#include "ctrl-lib/conf/ConfigManager.h"
#include <iostream>

namespace CalX {

	ConfigEntry::ConfigEntry(ConfigManager &conf, std::string name) : config(conf) {
		this->name = name;
	}

	const std::string &ConfigEntry::getEntryName() const {
		return this->name;
	}

	const ConfigurationValue &ConfigEntry::get(std::string id) const {
		if (this->content.count(id) != 0) {
			return this->content.at(id);
		} else {
			return ConfigurationValue::Empty;
		}
	}

	bool ConfigEntry::has(std::string id) const {
		return this->content.count(id) != 0;
	}

	bool ConfigEntry::put(std::string id, ConfigurationValue value) {
		bool change = false;
		if (this->has(id)) {
			this->content.erase(id);
			change = true;
		}
		if (value.is(ConfigValueType::None)) {
			return false;
		}
		this->content[id] = value;
		for (const auto &l : this->config.getEventListeners()) {
			if (change) {
				l->keyChanged(&this->config, this, id);
			} else {
				l->keyAdded(&this->config, this, id);
			}
		}
		return true;
	}

	bool ConfigEntry::remove(std::string id) {
		if (this->content.count(id) == 0) {
			return false;
		}
		this->content.erase(id);
		for (const auto &l : this->config.getEventListeners()) {
			l->keyRemoved(&this->config, this, id);
		}
		return true;
	}

	bool ConfigEntry::is(std::string id, ConfigValueType type) const {
		if (this->content.count(id) == 0) {
			return false;
		}
		ConfigurationValue val = this->content.at(id);
		return val.is(type);
	}

	int_conf_t ConfigEntry::getInt(std::string key, int_conf_t def) const {
		return this->get(key).getInt(def);
	}

	real_conf_t ConfigEntry::getReal(std::string key, real_conf_t def) const {
		return this->get(key).getReal(def);
	}

	bool ConfigEntry::getBool(std::string key, bool def) const {
		return this->get(key).getBool(def);
	}

	std::string ConfigEntry::getString(std::string key, std::string def) const {
		return this->get(key).getString(def);
	}

	void ConfigEntry::visit(ConfigEntryVisitor &visitor) const {
		for (const auto &kv : this->content) {
			visitor.visit(kv.first, kv.second);
		}
	}

	void ConfigEntry::visit(std::function<void (const std::string &, const ConfigurationValue &)> visit) const {
		for (const auto &kv : this->content) {
			visit(kv.first, kv.second);
		}
	}
}