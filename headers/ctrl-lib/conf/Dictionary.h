#ifndef CALX_CTRL_LIB_CONF_DICTIONARY_H_
#define CALX_CTRL_LIB_CONF_DICTIONARY_H_

#include "ctrl-lib/conf/Value.h"

namespace CalX {

  class ConfiguationFlatDictionary {
   public:
    virtual ~ConfiguationFlatDictionary() = default;
    virtual const ConfigurationValue &get(const std::string &) const = 0;
    virtual bool has(const std::string &) const = 0;
    virtual bool put(const std::string &, const ConfigurationValue &) = 0;
    virtual bool remove(const std::string &) = 0;
		virtual void visit(std::function<void (const std::string &, const ConfigurationValue &)>) const = 0;

		bool is(const std::string &, ConfigurationValueType) const;
		int_conf_t getInt(const std::string &, int_conf_t = 0) const;
		real_conf_t getReal(const std::string &, real_conf_t = 0) const;
		bool getBool(const std::string &, bool = false) const;
		std::string getString(const std::string &, const std::string & = "") const;
  };

  class ConfigurationCatalogue {
   public:
    virtual ~ConfigurationCatalogue() = default;
    virtual ConfiguationFlatDictionary *getEntry(const std::string &, bool) = 0;
    virtual bool hasEntry(const std::string &) const = 0;
    virtual bool removeEntry(const std::string &) = 0;
		virtual void visit(std::function<void (const std::string &, ConfiguationFlatDictionary &)>) const = 0;
  };
}

#endif