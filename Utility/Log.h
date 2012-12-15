#pragma once

#include "Logger.h"


#define LOG_ABC(category, ...) getLogger(category).logMessage(__VA_ARGS__)


enum class LogCategory
{
  EngineCore,
  Serialization,
  Utility,
};

void createLoggerForCategory(LogCategory category, const char* logFileFileName);

Logger& getLogger(LogCategory category);
