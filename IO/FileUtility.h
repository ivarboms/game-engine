#pragma once

#include <time.h>
#include <string>
#include <boost/date_time/posix_time/posix_time_types.hpp>


struct FileInfo
{
  //Does the file exist?
  bool exists;
  //The size of the file, in bytes.
  long long fileSizeBytes;
  //When the file was last accessed.
  boost::posix_time::ptime lastAccessedTime;
  //When the file was last modified.
  boost::posix_time::ptime lastModifiedTime;
  //When the file was created.
  boost::posix_time::ptime createdTime;
};


FileInfo getFileInfo(const wchar_t* fileName);

bool fileExists(const wchar_t* fileName);

bool directoryExists(const wchar_t* directoryName);

std::string readWholeFile(const char* fileName);
