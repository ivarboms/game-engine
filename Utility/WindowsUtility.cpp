#include "WindowsUtility.h"

#include <Windows.h>

std::wstring winApiErrorCodeToString(DWORD errorCode)
{
  wchar_t* buffer = nullptr;
  FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
    FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, errorCode,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (wchar_t*)&buffer, 0, nullptr);

  const std::wstring errorMessage(buffer, buffer + wcslen(buffer));

  LocalFree(buffer);

  return errorMessage;
}
