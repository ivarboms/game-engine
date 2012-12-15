#pragma once

#include "Log.h"

#include <boost/format.hpp>

#include <windows.h>


/*  Assert a condition. If the assertion fails, a message is displayed and logged.
*/
#define ASSERT2(condition, message)                                                                 \
do                                                                                                  \
{                                                                                                   \
__pragma(warning(push))                                                                             \
__pragma(warning(disable:4127))                                                                     \
  if (!(condition))                                                                                 \
  {                                                                                                 \
    boost::format formatedMessage("Assertion failed: %1%\n" /*condition*/                           \
      "%2%\n" /*message*/                                                                           \
      "%3%(%4%)\n" /*File + line number, clickable in VS output*/                                   \
      "Function: %5%");/*Function name*/                                                            \
    formatedMessage % #condition % message % __FILE__ % __LINE__ % __FUNCTION__;                    \
                                                                                                    \
    /*Log it to file.*/                                                                             \
    Logger& logger = getLogger(LogCategory::Utility);                                               \
    logger.logMessage(formatedMessage.str(), Logger::SEV_ERROR);                                    \
                                                                                                    \
    /*VS output window doesn't add a newline, must do it manually.*/                                \
    std::string messageStr(formatedMessage.str());                                                  \
    messageStr.push_back('\n');                                                                     \
    OutputDebugStringA(messageStr.c_str());                                                         \
                                                                                                    \
    messageStr += "\nDo you want to break? Cancel closes the application.";                         \
    const int messageBoxResult = MessageBoxA(nullptr, messageStr.c_str(), "Assertion Failed",       \
      MB_ICONERROR | MB_YESNOCANCEL);                                                               \
    if (messageBoxResult == IDYES)                                                                  \
    {                                                                                               \
      __debugbreak();                                                                               \
    }                                                                                               \
    else if (messageBoxResult == IDCANCEL)                                                          \
    {                                                                                               \
      exit(1);                                                                                      \
    }                                                                                               \
  }                                                                                                 \
}                                                                                                   \
while (false)                                                                                       \
__pragma(warning(pop))
