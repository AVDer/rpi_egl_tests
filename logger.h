#ifndef RPI3_LOGGER_H
#define RPI3_LOGGER_H

#include <cstdio>

class Logger {

public:
  static Logger& logger() {
    static Logger instance;
    return instance;
  }

  template<typename... I>
  static void error(I... items) {
    printf("Error: "); printf(items...); printf("\n");
  }

  template<typename... I>
  static void info(I... items) {
    printf("Info: "); printf(items...); printf("\n");
  }

  template<typename... I>
  static void debug(I... items) {
    printf("Debug: "); printf(items...); printf("\n");
  }

private:
  Logger();
};

#endif