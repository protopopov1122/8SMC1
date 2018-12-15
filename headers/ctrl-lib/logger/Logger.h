#ifndef CALX_CTRL_LIB_LOGGER_LOGGER_H_
#define CALX_CTRL_LIB_LOGGER_LOGGER_H_

#include "ctrl-lib/logger/Journal.h"

namespace CalX {

  class DefaultJournal : public ConfigurableJournal {
   public:
    DefaultJournal(LoggingSeverity = LoggingSeverity::Debug);
    JournalLogger &getSession() override;
    void closeSession() override;
    JournalLoggerController &getSessionController() override;
   private:
    std::unique_ptr<JournalSession> session;
    LoggingSeverity defaultSeverity;
  };
}

#endif