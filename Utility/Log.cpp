#include "Log.h"
#include "Logger.h"
#include "Assert2.h"

#include <concurrent_unordered_map.h>


Concurrency::concurrent_unordered_map<LogCategory, Logger> categorizedLoggers;


void createLoggerForCategory(LogCategory category, const char* logFileFileName)
{
  bool succeeded = categorizedLoggers.insert(
    std::make_pair<LogCategory, Logger>(std::move(category), Logger(logFileFileName))).second;
  ASSERT2(succeeded, "Failed to create logger.");
}

Logger& getLogger(LogCategory category)
{
  auto logger = categorizedLoggers.find(category);
  ASSERT2(logger != categorizedLoggers.end(), "The category has not been created.");

  return logger->second;
}
