#ifndef RPI3_LOGGER_H
#define RPI3_LOGGER_H

#include <cstdio>

class Logger {

public:

  enum LoggingLevel {
    LoggingTrace = 0,
    LoggingDebug,
    LoggingInfo,
    LoggingWarning,
    LoggingError
  };

  static Logger& logger() {
    static Logger instance;
    return instance;
  }

  static void set_logging_level(LoggingLevel level) {logging_level_ = level;}

  template<typename... I>
  static void error(I... items) {
    if (logging_level_ <= LoggingError) {
      printf("Error: "); printf(items...); printf("\n");
    }
  }

  template<typename... I>
  static void info(I... items) {
    if (logging_level_ <= LoggingInfo) {
      printf("Info: "); printf(items...); printf("\n");
    }
  }

  template<typename... I>
  static void debug(I... items) {
    if (logging_level_ <= LoggingDebug) {
      printf("Debug: "); printf(items...); printf("\n");
    }
  }

  template<typename... I>
  static void trace(I... items) {
    if (logging_level_ <= LoggingTrace) {
      printf("Trace: "); printf(items...); printf("\n");
    }
  }

private:
  Logger();

  static LoggingLevel logging_level_;
};

#endif