#include "ctrl-lib/logger/Logger.h"
#include "ctrl-lib/logger/Session.h"

namespace CalX {

  class DefaultJournalSession : public JournalDefaultLogger, public JournalSession {
   public:
    using JournalDefaultLogger::JournalDefaultLogger;

    JournalLogger &getLogger() override {
      return *this;
    }
    
    JournalLoggerController &getController() override {
      return *this;
    }
  };

  DefaultJournal::DefaultJournal(LoggingSeverity severity)
    : session(nullptr), defaultSeverity(severity) {}
  
  JournalLogger &DefaultJournal::getSession() {
    if (this->session == nullptr) {
      this->session = std::make_unique<DefaultJournalSession>(this->defaultSeverity);
    }
    return this->session->getLogger();
  }

  void DefaultJournal::closeSession() {
    this->session = nullptr;
  }

  JournalLoggerController &DefaultJournal::getSessionController() {
    this->getSession();
    return this->session->getController();
  }
}