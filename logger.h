#ifndef RPI3_LOGGER_H
#define RPI3_LOGGER_H

#include <cstdio>
#include <ctime>
#include <sys/time.h>
#include <thread>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

class Logger {

public:

  enum LoggingLevel {
    LoggingVerbose = -1,
    LoggingTrace,
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
      print_time();
      printf("%s", KRED);
      printf("Error: "); printf(items...); printf("\n");
      printf("%s", KNRM);
    }
  }

  template<typename... I>
  static void warning(I... items) {
    if (logging_level_ <= LoggingWarning) {
      print_time();
      printf("%s", KMAG);
      printf("Warn : "); printf(items...); printf("\n");
      printf("%s", KNRM);
    }
  }

  template<typename... I>
  static void info(I... items) {
    if (logging_level_ <= LoggingInfo) {
      print_time();
      printf("%s", KGRN);
      printf("Info : "); printf(items...); printf("\n");
      printf("%s", KNRM);
    }
  }

  template<typename... I>
  static void debug(I... items) {
    if (logging_level_ <= LoggingDebug) {
      print_time();
      printf("%s", KCYN);
      printf("Debug: "); printf(items...); printf("\n");
      printf("%s", KNRM);
    }
  }

  template<typename... I>
  static void trace(I... items) {
    if (logging_level_ <= LoggingTrace) {
      print_time();
      printf("%s", KWHT);
      printf("Trace: "); printf(items...); printf("\n");
      printf("%s", KNRM);
    }
  }

  template<typename... I>
  static void verbose(I... items) {
    if (logging_level_ <= LoggingVerbose) {
      print_time();
      printf("%s", KWHT);
      printf("Verb : "); printf(items...); printf("\n");
      printf("%s", KNRM);
    }
  }

private:
  Logger();
  

  void static print_time() {
    timeval curTime;
    gettimeofday(&curTime, nullptr);
    int milli = curTime.tv_usec / 1000;
    char buffer [80];
    strftime(buffer, 80, "%H:%M:%S", localtime(&curTime.tv_sec));
    char currentTime[84] = "";
    sprintf(currentTime, "%s:%03d", buffer, milli);
    printf("%s T:%lX ", currentTime, pthread_self());
  }

  static LoggingLevel logging_level_;
};

#endif