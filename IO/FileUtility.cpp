#include "stdafx.h"
#include "FileUtility.h"

#include <locale>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <filesystem>
#include <fstream>
#include <streambuf>
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

std::string readWholeFile(const char* fileName)
{
  std::ifstream t(fileName);
  t.seekg(0, std::ios::end);
  //Setting size to a minimum of 1 makes buffer[0] not crash in case of an error or empty file.
  const size_t size = std::max(static_cast<size_t>(t.tellg()), (size_t)1);
  t.seekg(0);

  std::string buffer(size, 0);
  t.read(&buffer[0], size);
  return buffer;
}
