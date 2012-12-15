#include "Logger.h"

#include <windows.h>


static const char* severityStrings[Logger::SEV_CRICICAL + 1] =
{
  "Debug   ",
  "Info    ",
  "Warning ",
  "Error   ",
  "Critical",
};


Logger::Logger(const char* fileName, Severity minSeverity /*= SEV_DEBUG*/, unsigned char messageFlags /*= 0*/)
  : logFile(nullptr)
  , minSeverity(minSeverity)
  , messageFlags((MessageFlag)messageFlags)
{
  if (fileName != nullptr)
  {
    fopen_s(&logFile, fileName, "wt");
  }
}

void Logger::logMessage(const std::string& message, Severity severity /*= SEV_INFO*/)
{
  if (severity < minSeverity)
  {
    return;
  }

  std::string messageHeader;
  if (messageFlags != 0)
  {
    messageHeader = generateFormatString(severity);
  }

  std::string combinedMessage;
  combinedMessage.reserve(message.size() + messageHeader.size() + 2);
  combinedMessage += messageHeader;
  combinedMessage += message;
  //Append newline at end.
  //combinedMessage.push_back('\r');
  combinedMessage.push_back('\n');

  if (logFile != nullptr)
  {
    fwrite(combinedMessage.c_str(), combinedMessage.size(), 1, logFile);
    fflush(logFile);
  }

  for (std::function<void(const std::string&)>& callback : messagedLoggedCallbacks)
  {
    callback(combinedMessage);
  }
}

std::string Logger::generateFormatString(Severity severity) const
{
  //Example string with every formatting option enabled:
  //[2012/11/25, 12:34:56:789, Critical, 0x12345]

  
  //Need 45 chars, add some more just to be safe.
  const size_t bufferSize = 64;
  std::string formatBuffer(bufferSize, '\0');

  //Characters written to formatBuffer.
  size_t charsWritten = 0;

  formatBuffer[0] = '[';
  ++charsWritten;

  //Get system time to we can provide a timestamp.
  SYSTEMTIME localTime;
  GetLocalTime(&localTime);

  //Date.
  if (messageFlags & DATE_ENABLED)
  {
    charsWritten += sprintf_s(&formatBuffer[charsWritten], bufferSize - charsWritten,
      "%04u/%02u/%02u", localTime.wYear, localTime.wMonth, localTime.wDay);
  }

  //Time (HH:MM:SS).
  if (messageFlags & (TIMESTAMP_SECS | TIMESTAMP_MILLISECS))
  {
    if (charsWritten != 1)
    {
      //Date has been written, need a comma.
      formatBuffer[charsWritten] = ',';
      formatBuffer[charsWritten + 1] = ' ';
      charsWritten += 2;
    }

    charsWritten += sprintf_s(&formatBuffer[charsWritten], bufferSize - charsWritten,
      "%02u:%02u:%02u", localTime.wHour, localTime.wMinute, localTime.wSecond);
  }
  //Milliseconds.
  if (messageFlags & TIMESTAMP_MILLISECS)
  {
    charsWritten += sprintf_s(&formatBuffer[charsWritten], bufferSize - charsWritten,
      ":%03u", localTime.wMilliseconds);
  }

  //Severity level.
  if (messageFlags & SEVERITY_AS_TEXT)
  {
    if (charsWritten != 1)
    {
      //Date or time has been written, need a comma.
      formatBuffer[charsWritten] = ',';
      formatBuffer[charsWritten + 1] = ' ';
      charsWritten += 2;
    }
    charsWritten += sprintf_s(&formatBuffer[charsWritten], bufferSize - charsWritten,
      severityStrings[severity]);
  }
  else if (messageFlags & SEVERITY_AS_INT)
  {
    if (charsWritten != 1)
    {
      //Date or time has been written, need a comma.
      formatBuffer[charsWritten] = ',';
      formatBuffer[charsWritten + 1] = ' ';
      charsWritten += 2;
    }
    charsWritten += sprintf_s(&formatBuffer[charsWritten], bufferSize - charsWritten,
      "%u", severity);
  }

  //Thread ID.
  if (messageFlags & THREAD_ID)
  {
    if (charsWritten != 1)
    {
      //Something has been written, need a comma.
      formatBuffer[charsWritten] = ',';
      formatBuffer[charsWritten + 1] = ' ';
      charsWritten += 2;
    }

    const unsigned long threadId = GetThreadId(GetCurrentThread());

    charsWritten += sprintf_s(&formatBuffer[charsWritten], bufferSize - charsWritten,
      "0x%05X", threadId);
  }

  formatBuffer[charsWritten] = ']';
  formatBuffer[charsWritten + 1] = ' ';

  //Erase extra characters.
  formatBuffer.erase(charsWritten + 2);

  return formatBuffer;
}
