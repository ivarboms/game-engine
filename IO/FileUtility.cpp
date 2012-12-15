#include "stdafx.h"
#include "FileUtility.h"

#include <locale>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <filesystem>


FileInfo getFileInfo(const wchar_t* fileName)
{
  struct _stat64 buffer;
  const int info = _wstat64(fileName, &buffer);

  FileInfo fileInfo = {};
  fileInfo.exists = info == 0;
  fileInfo.fileSizeBytes = buffer.st_size;

  return fileInfo;
}

bool fileExists(const wchar_t* fileName)
{
	//using namespace boost::filesystem;
  using namespace std::tr2::sys;
	return is_regular_file(wpath(fileName));
}

bool directoryExists(const wchar_t* directoryName)
{
  //using namespace boost::filesystem;
  using namespace std::tr2::sys;
	return is_directory(wpath(directoryName));
}
