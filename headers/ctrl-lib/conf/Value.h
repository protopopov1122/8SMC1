#ifndef CALX_CTRL_LIB_CONF_VALUE_H_
#define CALX_CTRL_LIB_CONF_VALUE_H_

#include "ctrl-lib/CtrlCore.h"
#include <variant>
#include <functional>

namespace CalX {

	typedef int64_t int_conf_t;
	typedef double real_conf_t;

	enum class ConfigurationValueType {
    Integer,
    Real,
    String,
    Boolean,
    None
  };

  class ConfiguationFlatDictionary; // Forward reference

	class ConfigurationValue {
	 public:
	 	ConfigurationValue();
		ConfigurationValue(const ConfigurationValue &);
		ConfigurationValue(int_conf_t);
		ConfigurationValue(real_conf_t);
		ConfigurationValue(bool);
		ConfigurationValue(std::string);

		ConfigurationValueType getType() const;
		bool is(ConfigurationValueType) const;

		int_conf_t getInt(int_conf_t = 0) const;
		real_conf_t getReal(real_conf_t = 0.0) const;
		bool getBool(bool = false) const;
		std::string getString(std::string = "") const;
		std::string toString() const;

		operator int_conf_t() const {
			return this->getInt();
		}

		operator real_conf_t() const {
			return this->getReal();
		}
		
		operator bool() const {
			return this->getBool();
		}
	
		operator std::string () const {
			return this->toString();
		}

		static ConfigurationValue Empty;
	 private:
		ConfigurationValueType type;
		std::variant<int_conf_t, real_conf_t, bool, std::string> value;
	};
}

#endif