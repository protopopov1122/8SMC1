#ifndef CALX_CTRL_LIB_LOGGER_LOGGER_H_
#define CALX_CTRL_LIB_LOGGER_LOGGER_H_

#include "ctrl-lib/logger/Journal.h"

namespace CalX {

  class DefaultJournal : public Journal {
   public:
    DefaultJournal(LoggingSeverity = LoggingSeverity::Debug);
    JournalLogger &openSession() override;
    void closeSession() override;
    JournalLoggerController &getSessionController();
   private:
    std::unique_ptr<JournalSession> session;
    LoggingSeverity defaultSeverity;
  };
}

#endif