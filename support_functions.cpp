#include "support_functions.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "logger.h"

size_t get_file_size(const std::string& filename) {
  struct stat file_stat;
  if (stat(filename.c_str(), &file_stat) == -1) {
    Logger::error("File: can't stat file %s", filename.c_str());
    return 0;
  }
  Logger::info("File: File %s has size %d bytes", filename.c_str(), file_stat.st_size);
  return file_stat.st_size;
}
