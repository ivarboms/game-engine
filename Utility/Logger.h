#pragma once

#include <string>
#include <functional>

#include <boost/format.hpp>
#include <boost/preprocessor.hpp>


struct Logger
{
  /*The severity of a log message.
  */
  enum Severity
  {
    //For debugging purposes.
    SEV_DEBUG = 0,

    //Non-essential information which may be useful for monitoring a system's health.
    SEV_INFO = 1,

    //Warning condition.
    SEV_WARNING = 2,

    //Normal behavior of system cannot be achieved, but system is likely to be able
    //to recover.
    SEV_ERROR = 3,

    //System is likely to fail.
    SEV_CRICICAL = 4,
  };

  /*Flags for configuring log message formatting.
  */
  enum MessageFlag
  {
    //YYYY/MM/DD
    DATE_ENABLED = 1 << 0,

    //HH:MM:SS
    TIMESTAMP_SECS = 1 << 1,
    //HH:MM:SS:MS0 where MS0 is milliseconds with 3 digits.
    TIMESTAMP_MILLISECS = (1 << 2) | TIMESTAMP_SECS,

    //Outputs severity level as text.
    SEVERITY_AS_TEXT = 1 << 3,
    //Outputs severity level as an int.
    SEVERITY_AS_INT = 1 << 4,

    //Output thread ID, format: 0x1234.
    THREAD_ID = 1 << 5,
  };


  /*  
  */
  Logger(const char* fileName, Severity minSeverity = SEV_DEBUG,
    unsigned char messageFlags = TIMESTAMP_MILLISECS | THREAD_ID);

  void logMessage(const std::string& message, Severity severity = SEV_INFO);


  //Magic to generate 10 overloads for 'write' with 1-10 arguments.
#define BOOST_PP_LOCAL_MACRO(n) \
  template<BOOST_PP_ENUM_PARAMS(n, typename T)> \
  void logMessage(const std::string& str BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(n, const T,& t), Severity severity = SEV_INFO) \
  { \
    logMessage(boost::str(boost::format(str) BOOST_PP_REPEAT(n, MAKE_FORMAT_ARGS, ~)), severity); \
  } \

#define MAKE_FORMAT_ARGS(z, n, unused) % t##n
#define BOOST_PP_LOCAL_LIMITS (1, 10)
#include BOOST_PP_LOCAL_ITERATE()
#undef MAKE_FORMAT_ARGS

  std::string generateFormatString(Severity severity) const;


  /*  The file messages are being logged to.
      May be null, in which case no messages are written to disk.
  */
  FILE* logFile;

  //Messages with lower severity than this will not be logged.
  Severity minSeverity;

  //The format used to log extra details, such as a time stamp or thread ID.
  MessageFlag messageFlags;

  /*  Whenever a message is logged, these callbacks are called with the log message.
      The callbacks may be called from different threads, and are therefore required to be thread safe.
  */
  std::vector<std::function<void(const std::string&)>> messagedLoggedCallbacks;
};
