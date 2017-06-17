#ifndef CALX_NCLI_CLI_PARAMETER_H_
#define CALX_NCLI_CLI_PARAMETER_H_

#include "CLICore.h"
#include <string>
#include <map>

namespace CalX {

  enum class NCLIParameterType {
    Integer, Float, String, Boolean, None
  };

  extern std::map<NCLIParameterType, std::string> TYPE_STRINGS;

  class NCLIParameter {
    public:
      NCLIParameter(NCLIParameterType);
      virtual ~NCLIParameter();

      NCLIParameterType getType();
      virtual std::string toString() = 0;
      virtual NCLIParameter *clone() = 0;
    private:
      NCLIParameterType type;
  };

  class NCLIInteger : public NCLIParameter {
    public:
      NCLIInteger(int64_t);
      virtual ~NCLIInteger();

      int64_t getValue();
      virtual std::string toString();
      virtual NCLIParameter *clone();
    private:
      int64_t value;
  };

  class NCLIFloat : public NCLIParameter {
    public:
      NCLIFloat(double);
      virtual ~NCLIFloat();

      double getValue();
      virtual std::string toString();
      virtual NCLIParameter *clone();
    private:
      double value;
  };

  class NCLIBoolean : public NCLIParameter {
    public:
      NCLIBoolean(bool);
      virtual ~NCLIBoolean();

      bool getValue();
      virtual std::string toString();
      virtual NCLIParameter *clone();
    private:
      bool value;
  };

  class NCLIString : public NCLIParameter {
    public:
      NCLIString(std::string);
      virtual ~NCLIString();

      std::string getValue();
      virtual std::string toString();
      virtual NCLIParameter *clone();
    private:
      std::string value;
  };

  class NCLINone : public NCLIParameter {
    public:
      NCLINone();
      virtual ~NCLINone();
      virtual std::string toString();
      virtual NCLIParameter *clone();
  };

  NCLIParameter *parseNCLIParameter(std::string);
}

#endif
