#include "stdafx.h"
#include "FileUtility.h"

#include <locale>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <filesystem>
#include <boost/date_time/posix_time/conversion.hpp>


FileInfo getFileInfo(const wchar_t* fileName)
{
  using namespace boost::posix_time;

  struct _stat64 buffer = {};
  const int info = _wstat64(fileName, &buffer);

  FileInfo fileInfo = {};
  fileInfo.exists = info == 0;
  fileInfo.fileSizeBytes = buffer.st_size;
  //If compilation fails here, the program is probably set to use 32 bit time_t.
  fileInfo.lastAccessedTime = from_time_t(buffer.st_atime);
  fileInfo.lastModifiedTime = from_time_t(buffer.st_mtime);
  fileInfo.createdTime = from_time_t(buffer.st_ctime);

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
