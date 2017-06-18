#include "ncli/CLIParameter.h"
#include <regex>

namespace CalX {

  std::map<NCLIParameterType, std::string> TYPE_STRINGS = {
    {NCLIParameterType::Integer, "integer"},
    {NCLIParameterType::Float, "float"},
    {NCLIParameterType::Boolean, "boolean"},
    {NCLIParameterType::String, "string"},
    {NCLIParameterType::None, "none"},
    {NCLIParameterType::Any, "any"}
  };

  NCLIParameter::NCLIParameter(NCLIParameterType tp)
    : type(tp) {}

  NCLIParameter::~NCLIParameter() {}

  NCLIParameterType NCLIParameter::getType() {
    return this->type;
  }

  int64_t NCLIParameter::as_int() {
    if (this->getType() == NCLIParameterType::Integer) {
      return ((NCLIInteger*) this)->getValue();
    } else {
      return 0;
    }
  }

  double NCLIParameter::as_float() {
    if (this->getType() == NCLIParameterType::Float) {
      return ((NCLIFloat*) this)->getValue();
    } else {
      return 0.0f;
    }
  }

  bool NCLIParameter::as_bool() {
    if (this->getType() == NCLIParameterType::Boolean) {
      return ((NCLIBoolean*) this)->getValue();
    } else {
      return false;
    }
  }

  std::string NCLIParameter::as_string() {
    if (this->getType() == NCLIParameterType::String) {
      return ((NCLIString*) this)->getValue();
    } else {
      return "";
    }
  }

  NCLIInteger::NCLIInteger(int64_t val)
    : NCLIParameter::NCLIParameter(NCLIParameterType::Integer), value(val) {}

  NCLIInteger::~NCLIInteger() {}

  int64_t NCLIInteger::getValue() {
    return this->value;
  }

  std::string NCLIInteger::toString() {
    return std::to_string(this->value);
  }

  NCLIParameter *NCLIInteger::clone() {
    return new NCLIInteger(this->value);
  }

  NCLIFloat::NCLIFloat(double val)
    : NCLIParameter::NCLIParameter(NCLIParameterType::Float), value(val) {}

  NCLIFloat::~NCLIFloat() {}

  double NCLIFloat::getValue() {
    return this->value;
  }

  std::string NCLIFloat::toString() {
    return std::to_string(this->value);
  }

  NCLIParameter *NCLIFloat::clone() {
    return new NCLIFloat(this->value);
  }

  NCLIBoolean::NCLIBoolean(bool val)
    : NCLIParameter::NCLIParameter(NCLIParameterType::Boolean), value(val) {}

  NCLIBoolean::~NCLIBoolean() {}

  bool NCLIBoolean::getValue() {
    return this->value;
  }

  NCLIParameter *NCLIBoolean::clone() {
    return new NCLIBoolean(this->value);
  }

  std::string NCLIBoolean::toString() {
    return this->value ? "true" : "false";
  }

  NCLIString::NCLIString(std::string val)
    : NCLIParameter::NCLIParameter(NCLIParameterType::String), value(val) {}

  NCLIString::~NCLIString() {}

  std::string NCLIString::getValue() {
    return this->value;
  }

  std::string NCLIString::toString() {
    return "'" + this->value + "'";
  }

  NCLIParameter *NCLIString::clone() {
    return new NCLIString(this->value);
  }

  NCLINone::NCLINone()
    : NCLIParameter::NCLIParameter(NCLIParameterType::None) {}

  NCLINone::~NCLINone() {}

  std::string NCLINone::toString() {
    return "none";
  }

  NCLIParameter *NCLINone::clone() {
    return new NCLINone();
  }

  NCLIParameter *parseNCLIParameter(std::string line) {
    std::regex int_regex("[-+]?[0-9]+");
    std::regex float_regex("[-+]?[0-9]*\\.?[0-9]+");
    if (std::regex_match(line, int_regex)) {
      return new NCLIInteger(std::stoll(line));
    } else if (std::regex_match(line, float_regex)) {
      return new NCLIFloat(std::stold(line));
    } else if (line.compare("true") == 0 || line.compare("false") == 0) {
      return new NCLIBoolean(line.compare("true") == 0);
    } else if (!line.empty() && line.at(0) == '\'' && line.at(line.size() - 1) == '\'') {
      return new NCLIString(line.substr(1, line.size() - 2));
    } else {
      return new NCLIString(line);
    }
  }

  bool NCLITypesCompatible(NCLIParameterType t1, NCLIParameterType t2) {
    return t1 == t2 || t2 == NCLIParameterType::Any;
  }
}
