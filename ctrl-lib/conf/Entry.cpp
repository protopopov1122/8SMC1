#include "ctrl-lib/conf/ConfigManager.h"
#include <iostream>

namespace CalX {

	ConfigEntry::ConfigEntry(ConfigManager &conf, const std::string &name)
		: config(conf), entryName(name) {}

	const ConfigurationValue &ConfigEntry::get(const std::string &id) const {
		if (this->content.count(id) != 0) {
			return this->content.at(id);
		} else {
			return ConfigurationValue::Empty;
		}
	}

	bool ConfigEntry::has(const std::string &id) const {
		return this->content.count(id) != 0;
	}

	bool ConfigEntry::put(const std::string &id, const ConfigurationValue &value) {
		bool change = false;
		if (this->has(id)) {
			this->content.erase(id);
			change = true;
		}
		if (value.is(ConfigurationValueType::None)) {
			return false;
		}
		this->content[id] = value;
		for (const auto &l : this->config.getEventListeners()) {
			if (change) {
				l->keyChanged(&this->config, this->entryName, id);
			} else {
				l->keyAdded(&this->config, this->entryName, id);
			}
		}
		return true;
	}

	bool ConfigEntry::remove(const std::string &id) {
		if (this->content.count(id) == 0) {
			return false;
		}
		this->content.erase(id);
		for (const auto &l : this->config.getEventListeners()) {
			l->keyRemoved(&this->config, this->entryName, id);
		}
		return true;
	}

	void ConfigEntry::visit(std::function<void (const std::string &, const ConfigurationValue &)> visit) const {
		for (const auto &kv : this->content) {
			visit(kv.first, kv.second);
		}
	}
}