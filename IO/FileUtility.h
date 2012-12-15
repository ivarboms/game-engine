#pragma once

#include <time.h>


struct FileInfo
{
	bool exists;
	long long fileSizeBytes;
  __time64_t lastAccessedTime;
  __time64_t lastModifiedTime;
  __time64_t createdTime;
};


FileInfo getFileInfo(const wchar_t* fileName);

bool fileExists(const wchar_t* fileName);

bool directoryExists(const wchar_t* directoryName);
